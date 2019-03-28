#pragma once


struct ControllerData {
  // Hat 0-255
  uint8_t leftHatX = 0;     // PS4.getAnalogHat(LeftHatX)
  uint8_t leftHatY = 0;     // PS4.getAnalogHat(LeftHatX)
  uint8_t rightHatX = 0;    // PS4.getAnalogHat(LeftHatX)
  uint8_t rightHatY = 0;    // PS4.getAnalogHat(LeftHatX)
  // level 0-15
  uint8_t batteryLevel = 0; // PS4.getBatteryLevel()
  // Analog buttons 0-255
  uint8_t L2 = 0;           // PS4.getAnalogButton(L2)
  uint8_t R2 = 0;           // PS4.getAnalogButton(R2)
  // buttons 0 - 1
  uint8_t triangle = 0;     // PS4.getButtonClick(TRIANGLE)
  uint8_t circle = 0;       // PS4.getButtonClick(CIRCLE)
  uint8_t cross = 0;        // PS4.getButtonClick(CROSS)
  uint8_t square = 0;       // PS4.getButtonClick(SQUARE)
  uint8_t up = 0;           // PS4.getButtonClick(UP)
  uint8_t down = 0;         // PS4.getButtonClick(DOWN)
  uint8_t right = 0;        // PS4.getButtonClick(RIGHT)
  uint8_t left = 0;         // PS4.getButtonClick(LEFT)
  bool connected = false;   // PS4.connected()
} __attribute__((packed, aligned(1)));
