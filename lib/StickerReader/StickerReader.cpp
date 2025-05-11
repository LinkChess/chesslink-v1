#include <Arduino.h>
#include "StickerReader.h"
#include "LEDUtils.h"

const int NUM_LEDS = 16;

// Move sticker database to RAM for debugging
const int sticker_values[][3] = {
    {12, 12, 13},  // Empty
    {83, 21, 22},  // Red
    {29, 72, 24},  // LightGreen
    {22, 39, 56},  // Blue
    {113, 131, 90}, // Gold
    {40, 79, 84},  // LightBlue
    {80, 55, 59},  // LightPink
    {150, 150, 150},  // Silver
    {88, 77, 24},  // Yellow
    {36, 24, 40},  // Purple
    // {100, 100, 100},  // White
    {72, 72, 72},  // White
    {21, 55, 25},  // Green
    {81, 36, 21}   // Orange
};

const char* sticker_labels[] = {
  "Empty", "Red", "LightGreen", "Blue", "Gold", "LightBlue", "LightPink",
  "Silver", "Yellow", "Purple", "White", "Green", "Orange"
};

const uint16_t WHITE_RAW_VALUES_ALL[4][16][3] = {
    // Board 1
    {
        {422, 257, 412},  // Sensor 0
        {273, 193, 246},  // Sensor 1
        {426, 327, 394},  // Sensor 2
        {359, 309, 324},  // Sensor 3
        {348, 311, 415},  // Sensor 4
        {318, 246, 342},  // Sensor 5
        {338, 246, 338},  // Sensor 6
        {207, 138, 208},  // Sensor 7
        {336, 315, 441},  // Sensor 8
        {391, 231, 344},  // Sensor 9
        {316, 214, 306},  // Sensor 10
        {381, 272, 399},  // Sensor 11
        {372, 219, 301},  // Sensor 12
        {361, 248, 339},  // Sensor 13
        {337, 276, 374},  // Sensor 14
        {306, 225, 364}   // Sensor 15
    },
  
    // Board 2
    {
        {367, 265, 347},  // Sensor 0
        {387, 301, 356},  // Sensor 1
        {420, 320, 457},  // Sensor 2
        {317, 229, 294},  // Sensor 3
        {468, 298, 459},  // Sensor 4
        {352, 208, 316},  // Sensor 5
        {387, 295, 362},  // Sensor 6
        {401, 254, 381},  // Sensor 7
        {375, 217, 335},  // Sensor 8
        {410, 242, 331},  // Sensor 9
        {458, 328, 428},  // Sensor 10
        {440, 305, 405},  // Sensor 11
        {539, 352, 422},  // Sensor 12
        {423, 289, 419},  // Sensor 13
        {512, 321, 523},  // Sensor 14
        {361, 258, 308}   // Sensor 15
    },
  
    // Board 3
    {
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
    },
  
    // Board 4
    {
        {568, 401, 529},
        // Sensor 1
        {408, 286, 386},
        // Sensor 2
        {546, 398, 550},
        // Sensor 3
        {388, 269, 364},
        // Sensor 4
        {469, 331, 433},
        // Sensor 5
        {345, 291, 437},
        // Sensor 6
        {539, 348, 452},
        // Sensor 7
        {375, 256, 369},
        // Sensor 8
        {340, 241, 364},
        // Sensor 9
        {384, 279, 379},
        // Sensor 10
        {345, 249, 308},
        // Sensor 11
        {398, 286, 361},
        // Sensor 12
        {425, 291, 400},
        // Sensor 13
        {297, 214, 267},
        // Sensor 14
        {327, 234, 273},
        // Sensor 15
        {350, 259, 383}
    }
  };
  

const char* piece_labels[] = {
    "Empty", "Red", "Green", "Blue", "Gold", "LightBlue", "LightPink",
    "Silver", "Yellow", "Purple", "White", "LightGreen", "Orange"
};

const char* piece_chars[] = {
    "-", "p", "P", "Q", "q", "r", "R", "B", "N", "b", "n", "K", "k"
};

const int STICKER_DB_SIZE = sizeof(sticker_values) / sizeof(sticker_values[0]);

StickerReader::StickerReader()
    : _lastAmbient(0), _controlA0(0), _controlA1(0), _controlA2(0), _controlA3(0),
      _led(nullptr), _sensorNumber(0), _lastAmbientTime(0) {}

void StickerReader::begin() {
    if (_led) {
        _lastAmbient = readAmbient();
        _lastAmbientTime = millis();
    }
}

void StickerReader::configure(int controlA0, int controlA1, int controlA2, int controlA3, int sensorNumber, CRGB &led) {
    _controlA0 = controlA0;
    _controlA1 = controlA1;
    _controlA2 = controlA2;
    _controlA3 = controlA3;
    _sensorNumber = sensorNumber;
    _led = &led;
}

void StickerReader::readSignature(int* out) {
    if (!_led) return;
    uint8_t controlBits = (_controlA0 << 0) | (_controlA1 << 1) | (_controlA2 << 2) | (_controlA3 << 3);
    PORTB = (PORTB & 0xF0) | controlBits;

    out[0] = readLEDSensors(255, 0, 0, *_led);
    out[1] = readLEDSensors(0, 255, 0, *_led);
    out[2] = readLEDSensors(0, 0, 255, *_led);

    // Determine board region index based on REGION value
    int rawIndex = 0; // Default to region A

    if (strcmp(REGION, "B") == 0) {
        rawIndex = 1;
    } else if (strcmp(REGION, "C") == 0) {
        rawIndex = 2;
    } else if (strcmp(REGION, "D") == 0) {
        rawIndex = 3;
    }
    // For 'A', REGION remains 0

    uint16_t rCal = WHITE_RAW_VALUES_ALL[rawIndex][_sensorNumber][0];
    uint16_t gCal = WHITE_RAW_VALUES_ALL[rawIndex][_sensorNumber][1];
    uint16_t bCal = WHITE_RAW_VALUES_ALL[rawIndex][_sensorNumber][2];

    float rFloat = static_cast<float>(out[0]);
    float gFloat = static_cast<float>(out[1]);
    float bFloat = static_cast<float>(out[2]);

    out[0] = static_cast<int>((rFloat / rCal) * 100);
    out[1] = static_cast<int>((gFloat / gCal) * 100);
    out[2] = static_cast<int>((bFloat / bCal) * 100);

    memcpy(_currentSignature, out, sizeof(_currentSignature[0]) * 3);
}

int StickerReader::readAmbient() {
    if (!_led) return 0;
    uint8_t controlBits = (_controlA0 << 0) | (_controlA1 << 1) | (_controlA2 << 2) | (_controlA3 << 3);
    PORTB = (PORTB & 0xF0) | controlBits;
    int ambient = readAmbientLight(*_led);
    _lastAmbient = ambient;
    _lastAmbientTime = millis();
    return ambient;
}

int StickerReader::checkAmbientChange(int threshold) {
    if (!_led) return 0;
    int currentAmbient = readAmbientLight(*_led);
    if (currentAmbient > threshold && _lastAmbient <= threshold) {
        _lastAmbient = currentAmbient;
        return -1;
    } else if (currentAmbient <= threshold) {
        _lastAmbient = currentAmbient;
        return 1;
    } else {
        return 0;
    }
}

int StickerReader::distance(int* a, const int* b) {
    float weights[3] = {1.0, 1.0, 1.0};
    float sum = 0;
    for (int i = 0; i < 3; i++) {
        float d = a[i] - b[i];
        sum += weights[i] * d * d;
    }
    return sqrt(sum);
}

const char* StickerReader::getFENFromLabel(const char* label) {
    for (int i = 0; i < sizeof(piece_labels) / sizeof(piece_labels[0]); i++) {
        if (strcmp(label, piece_labels[i]) == 0) {
            return piece_chars[i];
        }
    }
    return "?";
}

const char* StickerReader::identifySticker() {
    if (!_led) return "Error";
    const char* closestLabel = "Empty";
    int minDist = 1000000;
    int currentValues[3];
    for (int i = 0; i < STICKER_DB_SIZE; i++) {
        memcpy(currentValues, &sticker_values[i], sizeof(currentValues));
        int d = distance(_currentSignature, currentValues);
        if (d < minDist) {
            minDist = d;
            closestLabel = sticker_labels[i];
        }
    }
    return closestLabel;
}

PieceType StickerReader::getPieceType() const { return _pieceType; }



