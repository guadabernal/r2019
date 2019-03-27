#pragma once
#include <Arduino.h>
#include "pidS.h"
#include "utility.h"

#define MOTOR_NONE        0
#define MOTOR_RESET_COUNT 1 
#define MOTOR_INIT        2 
#define MOTOR_INIT_FINE0  3
#define MOTOR_INIT_FINE1  4
#define MOTOR_TO_ANG      7

#define MOTOR_CCW 0
#define MOTOR_CW  1

#define MOTOR_CPR           3200
#define GEAR_FACTOR         6
#define ANGLE_COUNT_FACTOR  GEAR_FACTOR * MOTOR_CPR / 360.0

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

  void endStopPressed() {
    return !digitalRead(pinEndStop);
  }

  void setPWM(uint16_t pwm) {    
    if (checkEndStop && endStopPressed()) { 
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
    checkEndStop = false;
    done = false;
  }

  int16_t angleToCount(int16_t angle) { return angle * ANGLE_COUNT_FACTOR; } // 3200 counts per rev

  void goToAngle(int16_t angle, int16_t PWMMax, int16_t PWMMin, bool endStop = true) {
    Serial.print("Angle = ");
    Serial.print(angle);
    Serial.print(" Count = ");
    Serial.println(angleToCount(angle));
    pid.reset(angleToCount(angle), PWMMax, PWMMin); //CCCW  90deg   3200=360  1600=180  800=90
    mode = MOTOR_TO_ANG;
    checkEndStop = endStop;
    done = false;
  }

  void update() {
    switch (mode) {
      case MOTOR_INIT: {    
        if (endStopPressed()) {
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
        if (endStopPressed()) {
          stop();
          mode = MOTOR_NONE;
          done = true;
          break;
        }
        currentPWM = initFinePWM;
        currentDir = initDir;
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
  bool checkEndStop = true;
  

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
