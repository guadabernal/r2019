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
    pinMode(pinEnaB, INPUT);
  }

  void dir(int dir) {
    digitalWrite(pinEnaA, dir);
    digitalWrite(pinEnaB, 1 - dir);
  }

  int dir() { return counter > 0; }
  
  bool pinAStatus = false;
  bool pinBStatus = false;  
  int counter;  

  void updateA() {  
    pinA = !pinA;
    if(pinA && !pinB) counter++;
    else if(!pinA && pinB) counter++;
    else counter--;
  }

  void updateB() {
    pinB = !pinB;
    if(pinB && pinA) counter++;
    else if(!pinB && !pinA) counter++;
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