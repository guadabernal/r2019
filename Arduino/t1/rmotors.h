#pragma once
#include <Arduino.h>
#include "pins.h"
#include "dc_motor.h"


class RMotors {
public:
  enum MType { FR, FL, BR, BL}
  RMotors()
  : m{ (AERFR, BERFR, PWMRFR, INARFR, INBRFR, CSRFR, SIGFR)
     , (AERFL, BERFL, PWMRFL, INARFL, INBRFL, CSRFL, SIGFL)
     , (AERBR, BERBR, PWMRBR, INARBR, INBRBR, CSRBR, SIGBR)
     , (AERBL, BERBL, PWMRBL, INARBL, INBRBL, CSRBL, SIGBL) }
  {}

  void updateA(uint8_t motorId) {
    m[motorId].updateA();
  }
  
  void updateB(uint8_t motor) {
    m[motorId].updateB();
  }

  int getPinA(uint8_t motorId) {
    return m[motorId].getPinA();
  }
  
  int updateB(uint8_t motor) {
    return m[motorId].getPinB();
  }

  void setup() {
    for (uint8_t i = 0; i < 4; ++i)
      m[i].setupMotor();
  }

  bool allDone() {
    return m[0].isDone() && m[1].isDone() && m[2].isDone() && m[3].isDone();   
  }

  void initializeMotors()
  {
    m[BL].initialize(1, 80, 40, 500);
    m[BR].initialize(0, 80, 40, 500);  
    m[FL].initialize(0, 80, 40, 500);    
    m[FR].initialize(1, 80, 40, 500);
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
        updateMotors();
        delay(updateTime);
      }  
  }

  void resetPosition() {
    // goto endstop
    initializeMotors();
    waitForAll(1);
    delay(500);
    resetCounters();
    // setInitial 90 degress angle
    m[BL].goToAngle(8, 40,-40, false);
    m[BR].goToAngle(-10, 40,-40, false); 
    m[FL].goToAngle(-3, 40,-40, false); 
    m[FR].goToAngle(8, 40,-40, false);      
    waitForAll(1);
    delay(500);
    resetCounters();

    // setInitial 0 degress angle
    m[BL].goToAngle(90, 60,-60);  // -90,  50
    m[BR].goToAngle(-90, 60,-60); //  90, -50
    m[FL].goToAngle(-90, 60,-60); //  90, -50
    m[FR].goToAngle(90, 60,-60);  // -90,  50  
    waitForMotors(1);
    delay(500);
    resetCounters();
  }

  void ratePos() {
    float rotAngle = 50;
    m[FR].goToAngle(-rotAngle, 60,-60);  // -90,  50     
    m[FL].goToAngle(rotAngle, 60,-60); //  90, -50
    m[BR].goToAngle(rotAngle, 60,-60); //  90, -50
    m[BL].goToAngle(-rotAngle, 60,-60);  // -90,  50
    waitForMotors(1);
    delay(500);    
  }

  void angleDir(float angle) {
    if (abs(angle) > 50) return;
    m[FR].goToAngle(angle, 60,-60);  // -90,  50      
    m[FL].goToAngle(angle, 60,-60); //  90, -50
    m[BR].goToAngle(angle, 60,-60); //  90, -50
    m[BL].goToAngle(angle, 60,-60);  // -90,  50
  }

private:
  Motor m[4];  
};