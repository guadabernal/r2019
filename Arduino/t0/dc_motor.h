#pragma once
#include <Arduino.h>
#include "pidS.h"
#include "utility.h"

#define MOTOR_NONE        0
#define MOTOR_RESET_COUNT 1 
#define MOTOR_TO_COUNT    2
#define MOTOR_TO_SPEED    3

#define MOTOR_CCW 0
#define MOTOR_CW  1

#define MOTOR_CPR           1440
#define GEAR_FACTOR         1
#define ANGLE_COUNT_FACTOR  GEAR_FACTOR * MOTOR_CPR / 360.0

class Motor {
public:
  Motor(int pinA, int pinB, int pinPWM, int pinEnaA, int pinEnaB, int pinCS)
  : pinA(pinA), pinB(pinB), pinPWM(pinPWM), pinEnaA(pinEnaA), pinEnaB(pinEnaB), pinCS(pinCS)
  , pidV(3.5, 40, 0)
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

  //int32_t distanceToCount(float distance) { return distance  * DISTANCE_COUNT_FACTOR; }

  void goToSpeed(float speed) {
    mode = MOTOR_TO_SPEED;
    pidV.reset(speed, 1023,-1023);
    t0 = micros();
    oldCounter = counter;
    oldPWM = 0;
    totalT = 0;
    delay(1);
    update();
  }

  // void goToDistance(float distance, float speed) {
  //   pid.reset(distanceToCount(distance), PWMMax, PWMMin);
  //   mode = MOTOR_TO_COUNT;
  //   done = false;
  // }

  void update() {
    switch (mode) {
      case MOTOR_RESET_COUNT: {
        mode = MOTOR_NONE;
        resetCounter();
        break;
      }
      case MOTOR_NONE: { break; }
      case MOTOR_TO_SPEED: {
        t1 = micros();
        float dt = t1 - t0;
        totalT += dt * 1E-3;
        if (totalT > 2000) {
          currentPWM = 0;
          break;
        }
        
        float dCount = counter - oldCounter;
        float currentV = dCount / dt * (1E6 * 60) / MOTOR_CPR;
        float pwm = pidV.compute(currentV);
        Serial.print(totalT);
        Serial.print(" ");        
        Serial.print(currentV, 6);
        Serial.print(" ");        
        Serial.println(pwm, 6);

        currentDir = pwm < 0 ? MOTOR_CW : MOTOR_CCW;
        currentPWM = abs(pwm);
        oldPWM = currentPWM;
        oldCounter = counter;
        t0 = t1;        
        break;
      }
      case MOTOR_TO_COUNT: {
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
  PID<float, float> pidV;

  long t0, t1;
  uint32_t oldCounter;
  uint16_t oldPWM;
  float totalT;


};
