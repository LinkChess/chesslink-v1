#include <Arduino.h>
#include "StickerReader.h"
#include "LEDUtils.h"
#include <FastLED.h>
#include "CalibrationUtil.h"

#define NUM_LEDS 16
#define DATA_PIN 3

const int PHOTO_READ = A1;

const int PHOTO_IN_A0 = 8;
const int PHOTO_IN_A1 = 9; 
const int PHOTO_IN_A2 = 10;
const int PHOTO_IN_A3 = 11;

CRGB leds[NUM_LEDS];

// Create readers for all 16 squares (4x4 grid)
// Format: StickerReader(address_bit_0, address_bit_1, address_bit_2, address_bit_3, led)
// Row 1
StickerReader reader0(1, 1, 1, 0, 0, leds[0]);   // a1
StickerReader reader1(0, 1, 1, 0, 1, leds[1]);   // b1
StickerReader reader2(1, 0, 1, 0, 2, leds[2]);   // c1
StickerReader reader3(0, 0, 1, 0, 3, leds[3]);   // d1
// Row 2
StickerReader reader4(1, 1, 0, 0, 4, leds[4]);   // a2
StickerReader reader5(0, 1, 0, 0, 5, leds[5]);   // b2
StickerReader reader6(1, 0, 0, 0, 6, leds[6]);   // c2
StickerReader reader7(0, 0, 0, 0, 7, leds[7]);   // d2

// Row 3
StickerReader reader8(0, 0, 0, 1, 8, leds[8]);   // a3
StickerReader reader9(1, 0, 0, 1, 9, leds[9]);   // b3
StickerReader reader10(0, 1, 0, 1, 10, leds[10]); // c3
StickerReader reader11(1, 1, 0, 1, 11, leds[11]); // d3

// Row 4
StickerReader reader12(0, 0, 1, 1, 12, leds[12]); // a4
StickerReader reader13(1, 0, 1, 1, 13, leds[13]); // b4
StickerReader reader14(0, 1, 1, 1, 14, leds[14]); // c4
StickerReader reader15(1, 1, 1, 1, 15, leds[15]); // d4

// Array to hold all readers for easier iteration
StickerReader* readers[NUM_LEDS] = {
  &reader0, &reader1, &reader2, &reader3,
  &reader4, &reader5, &reader6, &reader7,
  &reader8, &reader9, &reader10, &reader11,
  &reader12, &reader13, &reader14, &reader15
};

void setup() {
  Serial.begin(9600);
  Serial.println("Chess Sticker Calibration Program");
  Serial.println("================================");
  Serial.println("This program will calibrate all sensors and generate code");
  Serial.println("that you can copy-paste into your main.cpp file.");
  Serial.println();
  
  // Set internal reference for more stable readings
  analogReference(INTERNAL);
  
  // Initialize photodiode input
  pinMode(PHOTO_READ, INPUT);

  // Initialize LED control pins
  pinMode(PHOTO_IN_A0, OUTPUT);
  pinMode(PHOTO_IN_A1, OUTPUT);
  pinMode(PHOTO_IN_A2, OUTPUT);
  pinMode(PHOTO_IN_A3, OUTPUT);

  // Configure LEDs
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 1000);
  
  // Turn off all LEDs
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
  
  // Initialize all readers
  // for (int i = 0; i < NUM_LEDS; i++) {
  //   readers[i]->begin();
  // }
  
  // Wait a moment before starting calibration
  delay(1000);
  
  // Run the calibration
  CalibrationUtil::performCalibration(readers, NUM_LEDS, leds);
  
  Serial.println("Calibration complete! Copy the code above into your main.cpp file.");
  Serial.println("Set RUN_CALIBRATION = false in main.cpp to use these values.");
}

void loop() {
  // Nothing to do in the loop for calibration
  delay(1000);
  
  // Optional: Flash an LED to show the program is still running
  static bool ledState = false;
  ledState = !ledState;
  
  if (ledState) {
    leds[0] = CRGB(0, 10, 0); // Dim green
  } else {
    leds[0] = CRGB(0, 0, 0);
  }
  FastLED.show();
}
