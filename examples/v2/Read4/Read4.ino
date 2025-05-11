#include <Arduino.h>
#include "StickerReader.h"
#include "LEDUtils.h"
#include <FastLED.h>

#define NUM_LEDS 16
#define DATA_PIN 3

const int PHOTO_READ = A1;

const int PHOTO_IN_A0 = 8;
const int PHOTO_IN_A1 = 9; 
const int PHOTO_IN_A2 = 10;
const int PHOTO_IN_A3 = 11;

CRGB leds[NUM_LEDS];

// Create readers for all four squares
// Pass reference to LED array element
StickerReader readerA(1, 1, 1, 0, leds[0]);
StickerReader readerB(0, 1, 1, 0, leds[1]);
StickerReader readerC(1, 0, 1, 0, leds[2]);
StickerReader readerD(0, 0, 1, 0, leds[3]);

// Helper function to print detailed information for each square
void printSquareInfo(StickerReader& reader, int* signature, const char* label) {
  Serial.print("  R: "); Serial.print(signature[0]);
  Serial.print(", G: "); Serial.print(signature[1]);
  Serial.print(", B: "); Serial.print(signature[2]);
  Serial.print(" => Detected: ");
  Serial.println(label);
}

void setup() {
  Serial.begin(9600);

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

  readerA.begin();
  readerB.begin();
  readerC.begin();
  readerD.begin();
}

// R: 144, G: 115, B: 176 
// 131, G: 115, B: 175 => Detected: White=====================================

void loop() {

  // Arrays to store signatures for each reader
  int signatureA[4], signatureB[4], signatureC[4], signatureD[4];
  
  // Read signatures for all squares
  readerA.readSignature(signatureA);
  readerB.readSignature(signatureB);
  readerC.readSignature(signatureC);
  readerD.readSignature(signatureD);
  
  // Identify stickers for all squares
  const char* labelA = readerA.identifySticker();
  const char* labelB = readerB.identifySticker();
  const char* labelC = readerC.identifySticker();
  const char* labelD = readerD.identifySticker();
  
  // Get FEN characters for each square
  const char* fenA = readerA.getFENFromLabel(labelA);
  const char* fenB = readerB.getFENFromLabel(labelB);
  const char* fenC = readerC.getFENFromLabel(labelC);
  const char* fenD = readerD.getFENFromLabel(labelD);
  
  
  // Print combined FEN
  Serial.print("Combined FEN: ");
  Serial.print(fenA);
  Serial.print(fenB);
  Serial.print(fenC);
  Serial.println(fenD);
  
  // Print detailed information for debugging
  Serial.println("Square A:");
  printSquareInfo(readerA, signatureA, labelA);
  
  Serial.println("Square B:");
  printSquareInfo(readerB, signatureB, labelB);
  
  Serial.println("Square C:");
  printSquareInfo(readerC, signatureC, labelC);
  
  Serial.println("Square D:");
  printSquareInfo(readerD, signatureD, labelD);
  
  Serial.println("=====================================");
  delay(1000);
}
