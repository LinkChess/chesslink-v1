#include <Wire.h>
#include <Arduino.h>

#define SDA 6
#define SCL 7

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA, SCL);
}

void loop() {
  Wire.requestFrom(0x08, 19); // Request up to 32 bytes from slave

  Serial.print("Received: ");
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();

  delay(1000);
}
