#pragma once
#include <Arduino.h>
#include "pidS.h"
#include "utility.h"


class RMotors {
public:
  enum MType { FR, FL, BR, BL};
  RMotors()
  : m{ {AEFR, BEFR, PWMFR, INAFR, INBFR, CSFR}
     , {AEFL, BEFL, PWMFL, INAFL, INBFL, CSFL}
     , {AEBR, BEBR, PWMBR, INABR, INBBR, CSBR}
     , {AEBL, BEBL, PWMBL, INABL, INBBL, CSBL} }
  {}

  void updateA(uint8_t motorId) {
    m[motorId].updateA();
  }
  
  void updateB(uint8_t motorId) {
    m[motorId].updateB();
  }

  int getPinA(uint8_t motorId) {
    return m[motorId].getPinA();
  }
  
  int getPinB(uint8_t motorId) {
    return m[motorId].getPinB();
  }

  void setup() {
    for (uint8_t i = 0; i < 4; ++i)
      m[i].setupMotor();
  }

  bool allDone() {
    return m[0].isDone() && m[1].isDone() && m[2].isDone() && m[3].isDone();   
  }

  void update() {
    for (uint8_t i = 0; i < 4; ++i)
      m[i].update();
  }

  void resetCounters() {
    for (uint8_t i = 0; i < 4; ++i)
      m[i].resetCounter();
  }

  void off() {
    for (uint8_t i = 0; i < 4; ++i)
      m[i].off();
  }

  void waitForAll(int updateTime = 1) {
      while(!allDone()) {
        update();
        delay(updateTime);
      }  
  }

  void goToSpeed(int motorId, float v) {
  	m[motorId].goToSpeed(v);
  }

  void goToSpeed(float v) {
    m[FR].goToSpeed(-v);
    m[FL].goToSpeed(v);
    m[BR].goToSpeed(-v);
    m[BL].goToSpeed(v);
  }

  void goToSpeed(float vl, float vr) {
    m[FR].goToSpeed(-vr);
    m[FL].goToSpeed(vl);
    m[BR].goToSpeed(-vr);
    m[BL].goToSpeed(vl);
  }

 private:
  Motor m[4];  
};
