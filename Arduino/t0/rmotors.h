#pragma once
#include <Arduino.h>
#include "pidS.h"
#include "utility.h"

struct Robot {
  enum { Normal, Rotate };
  float b = 214 ;
  float l2 = 237 / 2;
  float R;
  float dl; // in degress
  float dr; // in degress
  float angle = 0;
  float VR;
  float VL;
  float v = 0;
  uint8_t mode = Normal;

  Robot() {
    update();
  }

  void setMode(uint8_t m) {
    mode = m;
  }
  
  void  increaseAngle(float dAngle) {
    if (mode == Normal) {
      angle += dAngle;
      update();
    }
  }
  
  void setAngle(float a) {
    if (mode == Normal) {
      angle = a;
      update();
    }
  }

  void setVel(float vel) {
    v = vel;
    if (mode == Normal) {
      update();
    }
    if (mode == Rotate) {
      VL = v;
      VR = v;
    }
  }
  
  void update() {
    if (angle > 48) angle = 48;
    if (angle < -48) angle = -48;
    if (abs(angle) > 0.05) {
      float da = degToRad(angle);
      if (da > 0) {
        R = b / tan(da) + l2;
        if (abs(R) < 10000) {
          dl = radToDeg(atan(b / (R + l2)));
          dr = angle;
        }
        else {
          dl = 0;
          dr = 0;
        }
      }
      else {
        R = b / tan(da) - l2;
        if (abs(R) < 10000) {
          dr = radToDeg(atan(b / (R - l2)));
          dl = angle;
        }
        else {
          dr = 0;
          dl = 0;
        }
      }
      float cR = (R+l2) / (R-l2);
      if (cR > 1) {
        VL = -v;
        VR = -VL / cR;
      }
      else {
        VR = v;
        VL = -VR * cR;
      }          
    }
    else {
      angle = 0;
      VR = v;
      VL = -VR;     
    }
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

  void goToSpeed(float vl, float vr) {
    m[FR].goToSpeed(vr);
    m[FL].goToSpeed(vl);
    m[BR].goToSpeed(vr);
    m[BL].goToSpeed(vl);
  }
  
 private:
  Motor m[4];  
};
