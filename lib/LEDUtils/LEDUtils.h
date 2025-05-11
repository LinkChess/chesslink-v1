#ifndef LED_UTILS_H
#define LED_UTILS_H

#include <FastLED.h>

// Update function signatures to use references for LED parameter
int readLEDSensors(int r, int g, int b, CRGB &led);
                   
int readAmbientLight(CRGB &led);

// New functions for baseline subtraction
int averageReading(int numReadings = 3);

// // Calibrate a raw reading using baseline subtraction
// void calibrateRGB(int rawR, int rawG, int rawB, int darkR, int darkG, int darkB, 
//                   float scaleR, float scaleG, float scaleB, 
//                   int& calR, int& calG, int& calB);

#endif