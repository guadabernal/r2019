#pragma once
#include <Arduino.h>
#include "pidS.h"
#include "utility.h"

#define MOTOR_NONE        0
#define MOTOR_RESET_COUNT 1 
#define MOTOR_INIT        2 
#define MOTOR_INIT_FINE0  3
#define MOTOR_INIT_FINE1  4
#define MOTOR_INIT_FINE2  5
#define MOTOR_TO_ANG      7

#define MOTOR_CCW 0
#define MOTOR_CW  1

class Motor {
public:
  Motor(int pinA, int pinB, int pinPWM, int pinEnaA, int pinEnaB, int pinCS, int pinEndStop = 0)
  : pinA(pinA), pinB(pinB), pinPWM(pinPWM), pinEnaA(pinEnaA), pinEnaB(pinEnaB), pinCS(pinCS)
  , pinEndStop(pinEndStop), pid(3, 0, 0.08)
  {}

  void setupMotor() {
    pinMode(pinA, INPUT_PULLDOWN);
    pinMode(pinB, INPUT_PULLDOWN);
    pinMode(pinPWM, OUTPUT);
    pinMode(pinEnaA, OUTPUT);
    pinMode(pinEnaB, OUTPUT);
    pinMode(pinCS, INPUT);
    if (pinEndStop) pinMode(pinEndStop, INPUT);
  }

  void setDir(int dir) {
    digitalWrite(pinEnaA, dir);
    digitalWrite(pinEnaB, 1 - dir);
  }

  void off() {
    currentPWM = 0;
    digitalWrite(pinEnaA, LOW);
    digitalWrite(pinEnaB, LOW);
    analogWrite(pinPWM, currentPWM);    
  }

  void stop() {
    currentPWM = 0;
    digitalWrite(pinEnaA, HIGH);
    digitalWrite(pinEnaB, HIGH);  
    analogWrite(pinPWM, currentPWM);     
  }

  void setPWM(uint16_t pwm) {    
    if (mode != MOTOR_INIT_FINE0 && mode != MOTOR_INIT_FINE2 && !digitalRead(pinEndStop)) { 
      return;
    }
    analogWrite(pinPWM, pwm);
  }

  void initialize(int8_t dir, uint16_t pwm, uint16_t finePWM, uint16_t fineTime) {
    initDir = dir;
    initPWM = pwm;
    initFinePWM = finePWM;
    initFineTime = fineTime;
    mode = MOTOR_INIT;
    done = false;
  }

  int16_t angleToCount(int16_t angle) { return 6 * angle * (8.89); }

  void goToAngle(int16_t angle, int16_t PWMMax, int16_t PWMMin) {
    Serial.print("Angle = ");
    Serial.print(angle);
    Serial.print(" Count = ");
    Serial.println(angleToCount(angle));
    pid.reset(angleToCount(angle), PWMMax, PWMMin); //CCCW  90deg   3200=360  1600=180  800=90
    mode = MOTOR_TO_ANG;
    done = false;
  }

  void update() {
    switch (mode) {
      case MOTOR_INIT: {    
        if (!digitalRead(pinEndStop)) {
          stop();
          mode = MOTOR_INIT_FINE0;
          initFineTimer.reset(initFineTime);
          break;
        }
        currentPWM = initPWM;
        currentDir = initDir;
        break;
      }
      case MOTOR_INIT_FINE0: {
        if (initFineTimer) {
          stop();
          mode = MOTOR_INIT_FINE1;
          break;
        }
        currentPWM = initFinePWM;
        currentDir = 1 - initDir;
        break;
      }
      case MOTOR_INIT_FINE1: {
        if (!digitalRead(pinEndStop)) {
          stop();
          mode = MOTOR_INIT_FINE2;
          initFineTimer.reset(120);
          break;
        }
        currentPWM = initFinePWM;
        currentDir = initDir;
        break;
      }
      case MOTOR_INIT_FINE2: {
        if (initFineTimer) {
          stop();
          done = true;          
          mode = MOTOR_NONE;
          break;
        }
        currentPWM = initFinePWM;
        currentDir = 1 - initDir;
        break;
      }
      case MOTOR_RESET_COUNT: {
        mode = MOTOR_NONE;
        resetCounter();
        break;
      }
      case MOTOR_NONE: { break; }
      case MOTOR_TO_ANG: {
        float pwm = pid.compute(counter);
        if (pwm < 0)
          currentDir = MOTOR_CW;
        else
          currentDir = MOTOR_CCW;
        float diff = pid.getTarget() - counter;
        currentPWM = abs(pwm);
        if (currentPWM < 10) currentPWM = 0;
        if (abs(diff) < 10) {
          off();
          done = true;
        } else {
        Serial.print("counter = ");
        Serial.print(counter);
        Serial.print(" Target = ");
        Serial.println(pid.getTarget());
          
        }
        break; 
      }
    }
    setPWM(currentPWM);
    setDir(currentDir);
  }

  int getMode() { return mode; }
  int dir() { return counter > 0; }
  
  bool pinAStatus = false;
  bool pinBStatus = false;  
  int counter;  

  void resetCounter() { counter = 0; }
  
  void updateA() {
    int vB = digitalRead(pinB);
    int vA = digitalRead(pinA);
    if(vA && !vB) counter++;
    else if(!vA && vB) counter++;
    else counter--; 
  }

  void updateB() {
    int vA = digitalRead(pinA);
    int vB = digitalRead(pinB);
    if(vB && vA) counter++;
    else if(!vB && !vA) counter++;
    else counter--; 
  }

  bool isDone() { return done; }
  int getPinA() { return pinA; }
  int getPinB() { return pinB; }

private: 
  int pinA;
  int pinB;
  int pinPWM;
  int pinEnaA;
  int pinEnaB;
  int pinCS;
  int pinEndStop;

  int mode = 0;
  uint16_t currentPWM = 0;
  int8_t currentDir = MOTOR_CCW;
  

  // initMode
  int8_t initDir;
  uint16_t initPWM;
  uint16_t initFinePWM;
  uint16_t initFineTime;
  Timer initFineTimer;
  float initAngle;
  bool done = false;

  //PID Controller
  PID<int16_t, float> pid;
};
