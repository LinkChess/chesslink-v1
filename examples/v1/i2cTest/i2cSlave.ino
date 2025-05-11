#include <Arduino.h>
#include <Wire.h>

void requestEvent() {
  Wire.write("1111/2222/3333/4444"); 
}

void setup() {
  Wire.begin(0x08);

  pinMode(A4, INPUT);
  pinMode(A5, INPUT);

  Wire.onRequest(requestEvent);
}
void loop() {
  // valueToSend could be updated here
}
