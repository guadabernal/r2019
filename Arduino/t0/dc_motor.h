#pragma once
#include <Arduino.h>
#include "pidS.h"
#include "utility.h"

#define MOTOR_NONE        0
#define MOTOR_RESET_COUNT 1 
#define MOTOR_TO_COUNT    2
#define MOTOR_TO_SPEED    3

// CW < 0  CCW > 0
#define MOTOR_CCW 0
#define MOTOR_CW  1

#define WHEEL_DIAMETER      0.2032
#define MOTOR_RPM_MAX       90
#define MOTOR_CPR           1440
#define GEAR_FACTOR         1
#define PID_COUNT_MIN       7
#define ANGLE_COUNT_FACTOR  GEAR_FACTOR * MOTOR_CPR / 360.0
#define DISTANCE_COUNT_FACTOR MOTOR_CPR / (WHEEL_DIAMETER * 3.14159265)

class Motor {
public:
  Motor(int id, int pinA, int pinB, int pinPWM, int pinEnaA, int pinEnaB, int pinCS, int dirRot)
  : id(id), pinA(pinA), pinB(pinB), pinPWM(pinPWM), pinEnaA(pinEnaA), pinEnaB(pinEnaB), pinCS(pinCS), dirRot(dirRot)
  , pidV(0.8, 10, 0, 1023, -1023)
  , pidD(2, 0, 0.1, 40, -40)
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

  int32_t distanceToCount(float distance) { return distance  * DISTANCE_COUNT_FACTOR * dirRot; }

  void goToDistance(float rpm, float distance) {
    int32_t count = distanceToCount(distance);
    rpm = abs(rpm) * sgn(count);
    if (rpm == 0) { 
      mode = MOTOR_NONE;
      stop();
      pidV.reset();
      return;
    }
    done = false;
    mode = MOTOR_TO_COUNT;
    float ang_vel = rpm / 60; // rps
    dtPIDMin = PID_COUNT_MIN / float(MOTOR_CPR) / abs(ang_vel); // s
    if (abs(rpm) > MOTOR_RPM_MAX) rpm = MOTOR_RPM_MAX * sgn(rpm);
    pidV.setTarget(rpm);
    pidD.setTarget(count);
    goToCountTargetSpeed=rpm;

    t0 = micros();
    oldCounter = counter;    
  }

  void goToSpeed(float rpm) {
    if (rpm == 0) { 
      mode = MOTOR_NONE;
      stop();
      pidV.reset();
      return;
    }
    mode = MOTOR_TO_SPEED;
    float ang_vel = rpm / 60; // rps
    dtPIDMin = PID_COUNT_MIN / float(MOTOR_CPR) / abs(ang_vel); // s
    if (abs(rpm) > MOTOR_RPM_MAX) rpm = MOTOR_RPM_MAX * sgn(rpm);
    pidV.setTarget(rpm);
    t0 = micros();
    oldCounter = counter;
  }

  void update() {
    switch (mode) {
      case MOTOR_RESET_COUNT: {
        mode = MOTOR_NONE;
        resetCounter();
        break;
      }
      case MOTOR_NONE: { break; }
      case MOTOR_TO_SPEED: {
        float dt = micros() - t0;
        if(dt * 1E-6 < dtPIDMin) break;
        float currentV = (counter - oldCounter) / dt * (1E6 * 60) / MOTOR_CPR;
        float pwm = pidV.compute(currentV);
        currentDir = pwm < 0 ? MOTOR_CW : MOTOR_CCW;
        currentPWM = abs(pwm);
        oldCounter = counter;
//                Serial.print(id);
//        Serial.print(" ");
//        Serial.print(counter);
//        Serial.print(" ");
//        Serial.print(oldCounter);       
//        Serial.print(" ");
//        Serial.println(currentV);
        t0 = micros();        
        break;
      }
      case MOTOR_TO_COUNT: {        
        if (abs(counter - pidD.getTarget()) < 10) {
          off();
          mode = MOTOR_NONE;
          done = true;
          break;
        }        
        float dt = micros() - t0;        
        if(dt * 1E-6 < dtPIDMin) break;
        
        float targetVel = pidD.compute(counter);
        if (abs(targetVel) > abs(goToCountTargetSpeed)) targetVel = goToCountTargetSpeed;
        pidV.setTarget(targetVel, false);
        float currentV = (counter - oldCounter) / dt * (1E6 * 60) / MOTOR_CPR;
        
        float pwm = pidV.compute(currentV);
        currentDir = pwm < 0 ? MOTOR_CW : MOTOR_CCW;
        currentPWM = abs(pwm);
        oldCounter = counter;
//        Serial.print(id);
//        Serial.print(" ");
//        Serial.print(counter);
//        Serial.print(" ");
//        Serial.println(currentV);

        if (abs(counter - pidD.getTarget()) < 10) {
          off();
          break;
        }        
        t0 = micros();        
        break;
      }
    }
    setPWM(currentPWM);
    setDir(currentDir);
  }

  uint32_t getPWM() { return currentPWM; }
  uint32_t getDir() { return currentDir; }

  int getMode() { return mode; }
  int dir() { return counter > 0; }
  
  bool pinAStatus = false;
  bool pinBStatus = false;  
  int counter = 0;  

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
  int id;
  int pinA;
  int pinB;
  int pinPWM;
  int pinEnaA;
  int pinEnaB;
  int pinCS;
  int dirRot;

  int mode = 0;
  uint16_t currentPWM = 0;
  int8_t currentDir = MOTOR_CCW;

  bool done = false;

  //PID Speed Controller
  PID<float, float> pidV;
  float dtPIDMin = 0;
  long t0, t1;
  int32_t oldCounter;
  float totalT = 0;


  PID<float, float> pidD;
  float goToCountTargetSpeed = 0;
};
