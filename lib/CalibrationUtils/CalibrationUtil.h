#ifndef CALIBRATION_UTIL_H
#define CALIBRATION_UTIL_H

#include <Arduino.h>
#include "StickerReader.h"
#include <FastLED.h>

class CalibrationUtil {
public:
  // Function to run calibration on all sensors and print the calibration data
  static void performCalibration(StickerReader* readers[], int numReaders, CRGB leds[]);
  
  // Print the calibration data in a format that can be copy-pasted into main.cpp
  static void printCalibrationCode(int darkValues[][3], float scaleFactors[][3], int numReaders);
};

#endif