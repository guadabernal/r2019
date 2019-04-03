#pragma once
#include <Arduino.h>
#include "pins.h"
#include "dc_motor.h"

struct Robot {
  float b;
  float l;
  float R;
  float dl; // in degress
  float dr; // in degress
  void update(float dAngle) {
    if (dAngle == 0) return;
    float da = degToRad(dAngle);
    if (da > 0) {
      R = b / tan(da) + l / 2;
      dr = radToDeg(atan2(b, (R + l / 2)));
      dl = dAngle;
    }
    else {
      R = b / tan(da) - l / 2;
      dl = radToDeg(atan2(b, (R - l / 2)));
      dr = dAngle;
    }
  }
};

class RMotors {
public:
  enum MType { FR, FL, BR, BL};
  RMotors()
  : m{ {AERFR, BERFR, PWMRFR, INARFR, INBRFR, CSRFR, SIGFR}
     , {AERFL, BERFL, PWMRFL, INARFL, INBRFL, CSRFL, SIGFL}
     , {AERBR, BERBR, PWMRBR, INARBR, INBRBR, CSRBR, SIGBR}
     , {AERBL, BERBL, PWMRBL, INARBL, INBRBL, CSRBL, SIGBL} }
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
        update();
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
    m[BL].goToAngle(90, 80,-80);  // -90,  50
    m[BR].goToAngle(-90, 80,-80); //  90, -50
    m[FL].goToAngle(-90, 80,-80); //  90, -50
    m[FR].goToAngle(90, 80,-80);  // -90,  50  
    waitForAll(1);
    delay(500);
    off();
    resetCounters();
    setMaxMinAngles();
  }

  void rotatePos() {
    float rotAngle = 50; // fix this angle
    m[FR].goToAngle(-rotAngle, 80,-80);  // -90,  50     
    m[FL].goToAngle(rotAngle, 80,-80); //  90, -50
    m[BR].goToAngle(rotAngle, 80,-80); //  90, -50
    m[BL].goToAngle(-rotAngle, 80,-80);  // -90,  50
    waitForAll(1);
    delay(500);    
  }

  void angleDir(float a) {
    m[FR].goToAngle(a, 80,-80);  // -90,  50      
    m[FL].goToAngle(a, 80,-80); //  90, -50
    m[BR].goToAngle(-a, 80,-80); //  90, -50
    m[BL].goToAngle(-a, 80,-80);  // -90,  50
  }

  void deltaAngleDir(float dAngle) {
    m[FR].goToDeltaAngle(dAngle, 80,-80);  // -90,  50      
    m[FL].goToDeltaAngle(dAngle, 80,-80); //  90, -50
    m[BR].goToDeltaAngle(-dAngle, 80,-80); //  90, -50
    m[BL].goToDeltaAngle(-dAngle, 80,-80);  // -90,  50    
  }

  void deltaAngleDir(float dAngleL, float dAngleR) {
    m[FR].goToDeltaAngle(dAngleR, 80,-80);  // -90,  50      
    m[FL].goToDeltaAngle(dAngleL, 80,-80); //  90, -50
    m[BR].goToDeltaAngle(-dAngleR, 80,-80); //  90, -50
    m[BL].goToDeltaAngle(-dAngleL, 80,-80);  // -90,  50     
  }

  void setMaxMinAngles() {
    m[FR].setMaxMinAngles(-90, 50);
    m[FL].setMaxMinAngles(90, -50);
    m[BR].setMaxMinAngles(90, -50);
    m[BL].setMaxMinAngles(-90, 50);
  }


private:
  Motor m[4];  
};
