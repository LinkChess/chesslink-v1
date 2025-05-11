#include <Arduino.h>
#include "StickerReaderCalibrate.h"
#include "LEDUtils.h"

const int NUM_LEDS = 16;

// Example sticker database (calibrated manually later)
StickerSignature stickerDB_5mm_1mm[] = {
    {"Empty",  {12, 12, 15}},
    {"Red",   {60, 15, 20}},
    {"LightGreen", {24, 40, 20}},
    {"Blue",  {22, 39, 56}},
    {"Gold", {113, 71, 89}},
    {"LightBlue", {40, 45, 57}},
    {"LightPink", {70, 30, 50}},
    {"Magenta", {58, 14, 37}}, 
    {"Yellow", {33, 33, 14}},
    {"Purple", {26, 11, 25}},
    {"White", {59, 39, 49}},
    {"Green", {14, 30, 18}},
    {"Orange", {29, 15, 15}}
};

const int DARK_VALUES[NUM_LEDS][3] = {
    {0, 0, 0},  // Sensor 0
    {0, 0, 0},  // Sensor 1
    {0, 0, 0},  // Sensor 2
    {0, 0, 0},  // Sensor 3
    {0, 0, 0},  // Sensor 4
    {0, 0, 0},  // Sensor 5
    {0, 0, 0},  // Sensor 6
    {0, 0, 0},  // Sensor 7
    {0, 0, 0},  // Sensor 8
    {0, 0, 0},  // Sensor 9
    {0, 0, 0},  // Sensor 10
    {0, 0, 0},  // Sensor 11
    {0, 0, 0},  // Sensor 12
    {0, 0, 0},  // Sensor 13
    {0, 0, 0},  // Sensor 14
    {0, 0, 0}   // Sensor 15
  };

const int WHITE_RAW_VALUES[NUM_LEDS][3] = {
    {524, 328, 461},  // Sensor 0
    {367, 230, 288},  // Sensor 1
    {435, 343, 454},  // Sensor 2
    {441, 327, 394},  // Sensor 3
    {383, 295, 336},  // Sensor 4
    {360, 240, 297},  // Sensor 5
    {438, 313, 398},  // Sensor 6
    {366, 252, 375},  // Sensor 7
    {606, 455, 553},  // Sensor 8
    {233, 149, 193},  // Sensor 9
    {400, 294, 510},  // Sensor 10
    {310, 216, 281},  // Sensor 11
    {418, 348, 429},  // Sensor 12
    {290, 204, 260},  // Sensor 13
    {375, 291, 346},  // Sensor 14
    {341, 238, 357}   // Sensor 15
  };
  


// FEN mapping. White is uppercase, black is lowercase PNBRQK
FenMapping pieceMap[] = {
    {"Empty", "-"},
    {"Red",   "p"},
    {"Green", "P"},
    {"Blue",  "Q"},
    {"Gold",  "q"},
    {"LightBlue", "r"},
    {"LightPink", "R"},
    {"Magenta", "B"},
    {"Yellow", "N"},
    {"Purple", "b"},
    {"White", "n"},
    {"LightGreen", "K"},
    {"Orange", "k"},
};

const int STICKER_DB_SIZE = sizeof(stickerDB_5mm_1mm) / sizeof(StickerSignature);

StickerReader::StickerReader(int controlA0, int controlA1, int controlA2, int controlA3, int sensorNumber, CRGB &led)
    : _lastAmbient(0),
    _controlA0(controlA0),
    _controlA1(controlA1),
    _controlA2(controlA2),
    _controlA3(controlA3),
    _led(led),
    _sensorNumber(sensorNumber),
    _lastAmbientTime(0) {}

void StickerReader::begin() {
    // Initialize ambient light reading
    _lastAmbient = readAmbient();
    _lastAmbientTime = millis();
}

void StickerReader::readSignature(int* out) {
    // set control bits directly
    uint8_t controlBits = (_controlA0 << 0) | (_controlA1 << 1) | (_controlA2 << 2) | (_controlA3 << 3);

    // Clear the lower 4 bits of PORTB, then set new values
    PORTB = (PORTB & 0xF0) | controlBits;

    // out[0] = 0;
    out[0] = readLEDSensors(255, 0, 0, _led);   // R
    out[1] = readLEDSensors(0, 255, 0, _led); // G
    out[2] = readLEDSensors(0, 0, 255, _led);  // B

    out[0] = out[0] / WHITE_RAW_VALUES[_sensorNumber][0];
    out[1] = out[1] / WHITE_RAW_VALUES[_sensorNumber][1];
    out[2] = out[2] / WHITE_RAW_VALUES[_sensorNumber][2];
}

// Read current ambient light level
int StickerReader::readAmbient() {
    // set control bits directly
    uint8_t controlBits = (_controlA0 << 0) | (_controlA1 << 1) | (_controlA2 << 2) | (_controlA3 << 3);

    // Clear the lower 4 bits of PORTB, then set new values
    PORTB = (PORTB & 0xF0) | controlBits;

    int ambient = readAmbientLight(_led);

    _lastAmbient = ambient;
    _lastAmbientTime = millis();
    return ambient;
}

// Check if ambient light has changed beyond threshold
// Return values: 0 = no significant change, 1 = piece placed, -1 = piece removed
int StickerReader::checkAmbientChange(int threshold) {
    int currentAmbient = readAmbientLight(_led);
    // int diff = currentAmbient - _lastAmbient;
    
    // Update the last ambient light reading regardless of change detection
    // _lastAmbientTime = millis();

    // Serial.print("[DEBUG] current ambient: ");
    // Serial.println(currentAmbient);

    // Return 1 if light increased (piece placed), -1 if decreased (piece removed), 0 if no significant change
    if (currentAmbient > threshold && _lastAmbient <= threshold) {
        _lastAmbient = currentAmbient;
        return -1;  // Piece removed (light increased)
    } else if (currentAmbient <= threshold) {
        _lastAmbient = currentAmbient;

        return 1; // Piece added (light decreased)
    } else {
        return 0;  // No significant change
    }
}

// Calculate the distance between two color signatures
// using a weighted Euclidean distance formula
// This is a simple approximation and can be adjusted
// based on the specific characteristics of the colors
// and the sensor used.
// The weights are chosen based on the sensitivity of the sensor
// to different colors. We may need to calibrate these
// weights based on your specific setup and the colors
// we are using.
int StickerReader::distance(int* a, int* b) {
    float weights[3] = {1.0, 1.0, 0.8}; // weights if needed for R, G, B
    float sum = 0;
    for (int i = 0; i < 3; i++) {
        float d = a[i] - b[i];
        sum += weights[i] * d * d;
    }
    return sqrt(sum);
}

const char* StickerReader::getFENFromLabel(const char* label) {
    for (int i = 0; i < sizeof(pieceMap) / sizeof(FenMapping); i++) {
      if (strcmp(label, pieceMap[i].label) == 0) {
        return pieceMap[i].piece;
      }
    }
    return "?";
  }

const char* StickerReader::identifySticker() {
    int sig[4];
    readSignature(sig);

    const char* closestLabel = "Processing";
    int minDist = 1000000;

    for (int i = 0; i < STICKER_DB_SIZE; i++) {
        int d = distance(sig, stickerDB_5mm_1mm[i].values);
        if (d < minDist) {
            minDist = d;
            closestLabel = stickerDB_5mm_1mm[i].label;
        }
    }

    return closestLabel;
}

PieceType StickerReader::getPieceType() const { return _pieceType; }