// New feature! Overclocking WS2812
// #define FASTLED_OVERCLOCK 1.2 // 20% overclock ~ 960 khz.
#include <Arduino.h>

// #define NUM_LEDS 8
// #define DATA_PIN 3
// CRGB leds[NUM_LEDS];

// void setup() { FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); }

// void loop() {
// 	leds[0] = CRGB(10, 0, 0); FastLED.show(); delay(30);
// 	leds[0] = CRGB::Black; FastLED.show(); delay(30);
// }

#include <FastLED.h>
#define NUM_LEDS 60
#define DATA_PIN 3

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(30); // optional brightness limit
}

void loop() {
  leds[0] = CRGB::Blue;

  // FastLED.show();
  // delay(500);

  // Turn them off
  for (int i = 0; i < 47; i++) {
    leds[i] = CRGB::Blue;
  }
  FastLED.show();
  delay(500);
}
