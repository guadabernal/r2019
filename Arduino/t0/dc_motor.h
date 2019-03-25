#pragma once
#include <Arduino.h>

class Motor {
public:
  Motor(int pinA, int pinB, int pinPWM, int pinEnaA, int pinEnaB, int pinCS)
  : pinA(pinA), pinB(pinB), pinPWM(pinPWM), pinEnaA(pinEnaA), pinEnaB(pinEnaB), pinCS(pinCS)
  {}

  void setupMotor() {
    pinMode(pinA, INPUT_PULLDOWN);
    pinMode(pinB, INPUT_PULLDOWN);
    pinMode(pinPWM, OUTPUT);
    pinMode(pinEnaA, OUTPUT);
    pinMode(pinEnaB, OUTPUT);
    pinMode(pinCS, INPUT);
  }

  void dir(int d) {
    digitalWrite(pinEnaA, d);
    digitalWrite(pinEnaB, 1 - d);
  }

  void off() {
    digitalWrite(pinEnaA, LOW);
    digitalWrite(pinEnaB, LOW);
    analogWrite(pinPWM, 0);    
  }

  void stop() {
    digitalWrite(pinEnaA, HIGH);
    digitalWrite(pinEnaB, HIGH);  
    analogWrite(pinPWM, 0);     
  }

  void speed(uint16_t pwm) {
    analogWrite(pinPWM, pwm);
  }

  int dir() { return counter > 0; }
  
  bool pinAStatus = false;
  bool pinBStatus = false;  
  int counter;  


  void resetCounter() {
    counter = 0;
  }
  
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

  int getPinA() {return pinA;}
  int getPinB() {return pinB;}

private: 
  int pinA;
  int pinB;
  int pinPWM;
  int pinEnaA;
  int pinEnaB;
  int pinCS;
};
