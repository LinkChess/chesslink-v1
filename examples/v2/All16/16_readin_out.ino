#include <Arduino.h>
#include "StickerReader.h"
#include "LEDUtils.h"
#include <FastLED.h>
#include <Wire.h>

#define NUM_LEDS 16
#define DATA_PIN 3
#define AMBIENT_LIGHT_THRESH 2
#define I2C_ADDRESS 0x13
#define BOARD_NAME 3

const int PHOTO_READ = A1;

const int PHOTO_IN_A0 = 8;
const int PHOTO_IN_A1 = 9; 
const int PHOTO_IN_A2 = 10;
const int PHOTO_IN_A3 = 11;

CRGB leds[NUM_LEDS];

// Replace String with char array to save RAM
char latestFEN[32] = "1111/2222/3333/4444";

void requestEvent() {
  // Send the latest FEN string when requested
  // Serial.println("i2c requested and sent");
  Wire.write(latestFEN);
}

// Create readers for all 16 squares (4x4 grid)
// Use a single reader and reuse it instead of creating 16 separate instances
StickerReader reader;

// Helper function to print detailed information for each square
void printSquareInfo(int* signature, const char* label) {
  Serial.print("  R: "); Serial.print(signature[0]);
  Serial.print(", G: "); Serial.print(signature[1]);
  Serial.print(", B: "); Serial.print(signature[2]);
  Serial.print(" => Detected: ");
  Serial.println(label);
}

void setup() {
  Serial.begin(9600);

  Wire.begin(I2C_ADDRESS);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  
  Wire.onRequest(requestEvent);

  analogReference(INTERNAL);
  
  // Initialize all readers
  pinMode(PHOTO_READ, INPUT);

  // configure leds 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(255); // optional brightness limit

  // set max voltage consumption of the leds
  FastLED.setMaxPowerInVoltsAndMilliamps(5,1000); 

  // Initialize all readers
  pinMode(PHOTO_IN_A0, OUTPUT);
  pinMode(PHOTO_IN_A1, OUTPUT);
  pinMode(PHOTO_IN_A2, OUTPUT);
  pinMode(PHOTO_IN_A3, OUTPUT);

  // Initialize reader once
  reader.begin();
}

void loop() {
  // Serial.println("Starting");

  // Arrays to store current signature and label
  int signature[4];
  const char* label;
  const char* fen;
  
  // Clear the FEN string
  memset(latestFEN, 0, sizeof(latestFEN));
  strcpy(latestFEN, "board");
  char boardNum[8];
  sprintf(boardNum, "%d", BOARD_NAME);
  strcat(latestFEN, boardNum);
  strcat(latestFEN, ":");
  
  // Process each square one at a time
  for (int i = 0; i < NUM_LEDS; i++) {
    // Configure reader for this specific square with the correct addressing
    uint8_t a0, a1, a2, a3;
    
    // Set multiplexer address bits according to the provided mapping
    switch(i) {
      case 0:  a0 = 1; a1 = 1; a2 = 1; a3 = 0; break; // a1
      case 1:  a0 = 0; a1 = 1; a2 = 1; a3 = 0; break; // b1
      case 2:  a0 = 1; a1 = 0; a2 = 1; a3 = 0; break; // c1
      case 3:  a0 = 0; a1 = 0; a2 = 1; a3 = 0; break; // d1
      case 4:  a0 = 1; a1 = 1; a2 = 0; a3 = 0; break; // a2
      case 5:  a0 = 0; a1 = 1; a2 = 0; a3 = 0; break; // b2
      case 6:  a0 = 1; a1 = 0; a2 = 0; a3 = 0; break; // c2
      case 7:  a0 = 0; a1 = 0; a2 = 0; a3 = 0; break; // d2
      case 8:  a0 = 0; a1 = 0; a2 = 0; a3 = 1; break; // a3
      case 9:  a0 = 1; a1 = 0; a2 = 0; a3 = 1; break; // b3
      case 10: a0 = 0; a1 = 1; a2 = 0; a3 = 1; break; // c3
      case 11: a0 = 1; a1 = 1; a2 = 0; a3 = 1; break; // d3
      case 12: a0 = 0; a1 = 0; a2 = 1; a3 = 1; break; // a4
      case 13: a0 = 1; a1 = 0; a2 = 1; a3 = 1; break; // b4
      case 14: a0 = 0; a1 = 1; a2 = 1; a3 = 1; break; // c4
      case 15: a0 = 1; a1 = 1; a2 = 1; a3 = 1; break; // d4
    }

    reader.configure(a0, a1, a2, a3, i, leds[i]);

    // comment this to keep on flashing
    if (reader.readAmbient() > AMBIENT_LIGHT_THRESH) {
      // If ambient light is too high, skip this square
      // Serial.print("Skipping square ");
      // Serial.println(i);
      strncat(latestFEN, "-", sizeof(latestFEN) - strlen(latestFEN) - 1);
      continue;
    }

    // Read signature and identify sticker
    reader.readSignature(signature);
    label = reader.identifySticker();
    fen = reader.getFENFromLabel(label);
    
    // Append to FEN string
    strncat(latestFEN, fen, sizeof(latestFEN) - strlen(latestFEN) - 1);
    
    // Print detailed information for debugging
    // Serial.print("Square ");
    // Serial.print(i);
    // Serial.println(":");
    // printSquareInfo(signature, label);
  }
  
  // Print combined FEN
  Serial.print("Combined FEN: ");
  
  Serial.println(latestFEN);
  
  Serial.println("=====================================");

  // modify the delay here for debug
  delay(1000);
}
