#include "CalibrationUtil.h"
#include "LEDUtils.h"

void CalibrationUtil::performCalibration(StickerReader* readers[], int numReaders, CRGB leds[]) {
  Serial.println("Starting sensor calibration...");

  Serial.println("\n// ---- COPY THE CODE BELOW INTO main.cpp ----");
  Serial.println("const int CALIBRATED_DARK_VALUES[NUM_LEDS][3] = {");

  Serial.println("/* DARK */ Cover with BLACK in 5 seconds...");

  delay(5000);

  // Step 1: DARK READING
  for (int i = 0; i < numReaders; i++) {
    Serial.print("// Sensor ");
    Serial.println(i);
    delay(500);

    uint8_t controlBits = (readers[i]->getControlA0() << 0) | 
                          (readers[i]->getControlA1() << 1) | 
                          (readers[i]->getControlA2() << 2) | 
                          (readers[i]->getControlA3() << 3);
    PORTB = (PORTB & 0xF0) | controlBits;

    leds[i] = CRGB(0, 0, 0);
    FastLED.show();

    delay(5); int r_dark = averageReading(5);
    delay(5); int g_dark = averageReading(5);
    delay(5); int b_dark = averageReading(5);

    Serial.print("  {"); Serial.print(r_dark); Serial.print(", ");
    Serial.print(g_dark); Serial.print(", ");
    Serial.print(b_dark); Serial.print("}");
    Serial.println(i < numReaders - 1 ? "," : "");
  }

  Serial.println("};");
  Serial.println("const float CALIBRATED_SCALE_FACTORS[NUM_LEDS][3] = {");

  Serial.println("/* WHITE */ Cover with WHITE in 5 seconds...");
  delay(5000);

  // Step 2: WHITE READING
  for (int i = 0; i < numReaders; i++) {
    Serial.print("// Sensor ");
    Serial.println(i);
    delay(500);

    uint8_t controlBits = (readers[i]->getControlA0() << 0) | 
                          (readers[i]->getControlA1() << 1) | 
                          (readers[i]->getControlA2() << 2) | 
                          (readers[i]->getControlA3() << 3);
    PORTB = (PORTB & 0xF0) | controlBits;

    leds[i] = CRGB(255, 0, 0); FastLED.show(); delay(5); int r_white = averageReading(5);
    leds[i] = CRGB(0, 255, 0); FastLED.show(); delay(5); int g_white = averageReading(5);
    leds[i] = CRGB(0, 0, 255); FastLED.show(); delay(5); int b_white = averageReading(5);
    leds[i] = CRGB(0, 0, 0);   FastLED.show();

    // You must reuse the same dark values from earlier; here we assume you noted them manually
    // or re-read them before this second pass (if not saving in memory).
    // int r_dark = 0; // Replace with correct value manually or read again
    // int g_dark = 0;
    // int b_dark = 0;

    // int r_diff = r_white - r_dark;
    // int g_diff = g_white - g_dark;
    // int b_diff = b_white - b_dark;

    // float r_scale = (r_diff > 10) ? 255.0f / r_diff : 1.0f;
    // float g_scale = (g_diff > 10) ? 255.0f / g_diff : 1.0f;
    // float b_scale = (b_diff > 10) ? 255.0f / b_diff : 1.0f;

    Serial.print("  {"); Serial.print(r_white); Serial.print(", ");
    Serial.print(g_white); Serial.print(", ");
    Serial.print(b_white); Serial.print("}");
    Serial.println(i < numReaders - 1 ? "," : "");
    Serial.println(i < numReaders - 1 ? "," : "");
  }

  Serial.println("};");
  Serial.println("// ---- END OF CALIBRATION CODE ----\n");
}