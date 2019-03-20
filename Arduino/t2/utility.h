#pragma once
#include <Arduino.h>

// deltaT [ms]
// mode 0: periodic timer 1: only once
class Timer {
public:
  Timer(int deltaT = 0, int mode = 0) 
  : t0(millis())
  , deltaT(deltaT)
  , done(false)
  , mode(mode)
  {}

  operator bool() {
    if (done && mode) return true;
    int t1 = millis();
    if (t1 - t0 > deltaT) {
      t0 = t1;
      done = true;
      return true;
    }
    return false;
  }

  void reset() {
    t0 = millis();
    done = false;
  }

  void reset(int dt) {
    deltaT = dt;
    t0 = millis();
    done = false;
  }

  void setPeriodic() {
    mode = 0;
  }

  void setOnlyOnce() {
    mode = 1;
  }
private:
  int t0;
  int t1;
  int deltaT;
  int mode; // 0: periodic timer 1: only once
  bool done;
};

template<typename T>
void Serial1Read<T>(T* p, uint16_t count = 1) {
    Serial1.readBytes((uint8_t*) p, count * sizeof(T));
}

template<typename T>
void Serial1Write<T>(T* p, uint16_t count = 1) {
    Serial1.write((uint8_t*) p, count * sizeof(T));
}
