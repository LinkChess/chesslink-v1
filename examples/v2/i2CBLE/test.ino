#include <Wire.h>
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SDA 6
#define SCL 7

// I2C addresses for the two Arduino Nanos
const uint8_t BOARD1_ADDR = 0x08;
// const uint8_t BOARD2_ADDR = 0x10;
// const uint8_t BOARD3_ADDR = 0x12;
// const uint8_t BOARD4_ADDR = 0x14;

// Global BLECharacteristic pointer
BLECharacteristic *pChessCharacteristic;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA, SCL);

  BLEDevice::init("ESP32C3_BLE");

  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService("12345678-1234-1234-1234-123456789abc");

  pChessCharacteristic = pService->createCharacteristic(
      "abcd1234-ab12-cd34-ef56-123456789abc",
      BLECharacteristic::PROPERTY_READ |
      BLECharacteristic::PROPERTY_NOTIFY
  );

  pChessCharacteristic->setValue("Chess boards connecting...");
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->start();

  Serial.println("BLE advertising...");
}

String readBoardData(uint8_t address) {
  Wire.requestFrom(address, 32); // Request up to 32 bytes
  
  String receivedData = "";
  while (Wire.available()) {
    char c = Wire.read();
    if (c >= 32 && c <= 126) { // Only add printable ASCII characters
      receivedData += c;
    }
  }
  return receivedData;
}

void loop() {
  // Read from both boards
  String board1Data = readBoardData(BOARD1_ADDR);
  delay(100); // Small delay between I2C requests

  // String board2Data = readBoardData(BOARD2_ADDR);
  // delay(100); // Small delay between I2C requests

  // String board3Data = readBoardData(BOARD3_ADDR);
  // delay(100); // Small delay between I2C requests

  // String board4Data = readBoardData(BOARD4_ADDR);
  // delay(100); // Small delay between I2C requests
  
  // Combine the data from both boards
  String combinedData = "";
  
  if (board1Data.length() > 0) {
    Serial.print("Board 1 data: ");
    Serial.println(board1Data);
    combinedData += board1Data + ";";
  }
  
  // if (board2Data.length() > 0) {
  //   Serial.print("Board 2 data: ");
  //   Serial.println(board2Data);
  //   combinedData += board2Data;
  // }

  // if (board3Data.length() > 0) {
  //   Serial.print("Board 3 data: ");
  //   Serial.println(board3Data);
  //   combinedData += board3Data;
  // }

  // if (board4Data.length() > 0) {
  //   Serial.print("Board 4 data: ");
  //   Serial.println(board4Data);
  //   combinedData += board4Data;
  // }
  
  // Send combined data via Bluetooth
  if (combinedData.length() > 0) {
    pChessCharacteristic->setValue(combinedData.c_str());
    pChessCharacteristic->notify();
    Serial.println("Combined data sent via Bluetooth");
  }
  
  delay(1000);
}
