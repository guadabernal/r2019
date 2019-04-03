#pragma once
#include <Arduino.h>
#include "pidS.h"
#include "utility.h"

struct Robot {
  float b = 214 ;
  float l2 = 237 / 2;
  float R;
  float dl; // in degress
  float dr; // in degress
  float angle = 0;
  
  void update(float dAngle) {
    angle  += dAngle;
    if (angle > 48) angle = 48;
    if (angle < -48) angle = -48;
    if (angle == 0) {
      
      return;
    }
    float da = degToRad(angle);
    if (da > 0) {
      R = b / tan(da) + l2;
      dl = radToDeg(atan(b / (R + l2)));
      dr = angle;
    }
    else {
      R = b / tan(da) - l2;
      dr = radToDeg(atan(b / (R - l2)));
      dl = angle;
    }
  }

  float getVL(float VR) {
    float VL = angle == 0 ? -VR : -VR * (R + l2) / (R - l2);
    Serial.print("angle = ");
    Serial.print(angle);
    Serial.print("VR = ");
    Serial.print(VR);
    Serial.print("VL = ");
    Serial.print(VL);
    Serial.print(" R=");
    Serial.println(R);
    return VL;
  }

  
};


class RMotors {
public:
  enum MType { FR, FL, BR, BL};
  RMotors()
  : m{ {0, AEFR, BEFR, PWMFR, INAFR, INBFR, CSFR}
     , {1, AEFL, BEFL, PWMFL, INAFL, INBFL, CSFL}
     , {2, AEBR, BEBR, PWMBR, INABR, INBBR, CSBR}
     , {3, AEBL, BEBL, PWMBL, INABL, INBBL, CSBL} }
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
    for (uint8_t i = 0; i < 4; ++i) {
      m[i].update();
    }
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

//  void goToSpeed(int motorId, float v) {
//  	m[motorId].goToSpeed(v);
//  }
//
//  void goToSpeed(float v) {
//    m[FR].goToSpeed(-v);
//    m[FL].goToSpeed(v);
//    m[BR].goToSpeed(-v);
//    m[BL].goToSpeed(v);
//  }

  void goToSpeed(float vl, float vr) {
    m[FR].goToSpeed(vr);
    m[FL].goToSpeed(vl);
    m[BR].goToSpeed(vr);
    m[BL].goToSpeed(vl);
  }
  
 private:
  Motor m[4];  
};
