#pragma once
#include <Arduino.h>
#include "pidS.h"
#include "utility.h"

#define MOTOR_NONE        0
#define MOTOR_RESET_COUNT 1 
#define MOTOR_TO_COUNT    2

#define MOTOR_CCW 0
#define MOTOR_CW  1

#define MOTOR_CPR           1440
#define GEAR_FACTOR         1
#define ANGLE_COUNT_FACTOR  GEAR_FACTOR * MOTOR_CPR / 360.0

class Motor {
public:
  Motor(int pinA, int pinB, int pinPWM, int pinEnaA, int pinEnaB, int pinCS)
  : pinA(pinA), pinB(pinB), pinPWM(pinPWM), pinEnaA(pinEnaA), pinEnaB(pinEnaB), pinCS(pinCS)
  , pid(3, 0, 0.08)
  {}

  void setupMotor() {
    pinMode(pinA, INPUT_PULLDOWN);
    pinMode(pinB, INPUT_PULLDOWN);
    pinMode(pinPWM, OUTPUT);
    pinMode(pinEnaA, OUTPUT);
    pinMode(pinEnaB, OUTPUT);
    pinMode(pinCS, INPUT);
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
    analogWrite(pinPWM, pwm);
  }

  int16_t angleToCount(int16_t angle) { return angle  * ANGLE_COUNT_FACTOR; }

  void goToAngle(int16_t angle, int16_t PWMMax, int16_t PWMMin) {
    Serial.print("Angle = ");
    Serial.print(angle);
    Serial.print(" Count = ");
    Serial.println(angleToCount(angle));
    pid.reset(angleToCount(angle), PWMMax, PWMMin); //CCCW  90deg   3200=360  1600=180  800=90
    mode = MOTOR_TO_COUNT;
    done = false;
  }

  void update() {
    switch (mode) {
      case MOTOR_RESET_COUNT: {
        mode = MOTOR_NONE;
        resetCounter();
        break;
      }
      case MOTOR_NONE: { break; }
      case MOTOR_TO_COUNT: {
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

  bool done = false;

  //PID Controller
  PID<int16_t, float> pid;
};
