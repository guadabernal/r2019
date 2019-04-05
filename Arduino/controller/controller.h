#pragma once
#include <Arduino.h>
#include <PS4BT.h>


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

	void update(PS4BT &PS4) {
    connected = PS4.connected();
    if (PS4.connected()) {
    	leftHatX = PS4.getAnalogHat(LeftHatX);
    	leftHatY = PS4.getAnalogHat(LeftHatY);
    	rightHatX = PS4.getAnalogHat(RightHatX);
    	rightHatY = PS4.getAnalogHat(RightHatY);
    	batteryLevel = PS4.getBatteryLevel();
    	L2 = PS4.getAnalogButton(L2);
    	R2 = PS4.getAnalogButton(R2);
    	triangle = PS4.getButtonPress(TRIANGLE);
    	circle = PS4.getButtonPress(CIRCLE);
    	cross = PS4.getButtonPress(CROSS);
    	square = PS4.getButtonPress(SQUARE);
    	up = PS4.getButtonPress(UP);
    	down = PS4.getButtonPress(DOWN);
    	right = PS4.getButtonPress(RIGHT);
    	left = PS4.getButtonPress(LEFT);
	  }
	}
} __attribute__((packed, aligned(1)));
