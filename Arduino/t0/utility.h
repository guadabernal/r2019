#pragma once
#include <Arduino.h>

class Timer {
public:
  Timer(int deltaT) : t0(millis()), deltaT(deltaT) {}
  operator bool() {
    int t1 = millis();
    if (t1 - t0 > deltaT) {
      t0 = t1;
      return true;
    }
    return false;
  }
  
private:
  int t0;
  int t1;
  int deltaT;
};
