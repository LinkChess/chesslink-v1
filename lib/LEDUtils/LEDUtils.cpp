#include <Arduino.h>
#include "LEDUtils.h"
#include "FastLED.h"

int averageReading(int numReadings) {
  long total = 0;
  for (int i = 0; i < numReadings; i++) {
    total += analogRead(A1);
    delayMicroseconds(50);  // Small delay between readings
  }
  return total / numReadings;
}

// // Apply calibration to raw readings
// void calibrateRGB(int rawR, int rawG, int rawB, int darkR, int darkG, int darkB, 
//                   float scaleR, float scaleG, float scaleB, 
//                   int& calR, int& calG, int& calB) {
//   // Apply baseline subtraction and scaling
//   float fR = (rawR - darkR) * scaleR;
//   float fG = (rawG - darkG) * scaleG;
//   float fB = (rawB - darkB) * scaleB;

//   // Clamp and cast to integer
//   calR = constrain((int)round(fR), 0, 255);
//   calG = constrain((int)round(fG), 0, 255);
//   calB = constrain((int)round(fB), 0, 255);
// }

int readLEDSensors(int r, int g, int b, CRGB &led) {
  // off
  led = CRGB(0, 0, 0);
  FastLED.show();
  delayMicroseconds(500);

  // Take average reading
  int off1 = averageReading();

  // set to on
  led = CRGB(r, g, b);
  FastLED.show();
  delayMicroseconds(500);
  
  int on = averageReading();

  // off
  led = CRGB(0, 0, 0);
  FastLED.show();
  delayMicroseconds(500);

  int off2 = averageReading();

  float ambient = (off1 + off2) / 2.0;
  int signal = on - ambient;

  return max(0, signal);
}

int readAmbientLight(CRGB &led) {
  // set control bits directly
  // uint8_t controlBits = (controlA0 << 0) | (controlA1 << 1) | (controlA2 << 2) | (controlA3 << 3);

  // // Clear the lower 4 bits of PORTB, then set new values
  // PORTB = (PORTB & 0xF0) | controlBits;

  // Make sure all LEDs are off
  led = CRGB(0, 0, 0);
  FastLED.show();
  delayMicroseconds(500); // delay(5);  // Add delay to ensure LED is fully off

  long total = 0;
  int numReadings = 3;  // You can adjust this based on how stable you need the reading

  // Take 'numReadings' readings and accumulate the results
  for (int i = 0; i < numReadings; i++) {
    total += analogRead(A1);
    // delayMicroseconds(500);  // Small delay between readings to allow sensor settling
  }

  // Calculate the average of the readings
  int ambient = total / numReadings;

  // Set the LED to the ambient light reading for debugging
  // Serial.print("Ambient Light Reading: ");
  // Serial.println(ambient);

  return ambient;
}
