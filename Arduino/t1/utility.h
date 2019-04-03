#pragma once
#include <Arduino.h>

#define MSG_OK 1

template<typename T>
void SerialRead(HardwareSerial &S, T* p, uint16_t count = 1) {
    while(S.available() < count * sizeof(T)) delay(1);
    S.readBytes((uint8_t*) p, count * sizeof(T));
}

template<typename T>
void SerialWrite(HardwareSerial &S, T* p, uint16_t count = 1) {
    S.write((uint8_t*) p, count * sizeof(T));
}

void SerialWriteOK(HardwareSerial &S) {
    uint8_t p = MSG_OK;
    S.write((uint8_t*) &p, 1);
}

bool SerialReadOK(HardwareSerial &S) {
    uint8_t p = 0;  
    SerialRead<uint8_t>(S, &p);
    return (p == MSG_OK);
}

template<typename T>
T clip(T v, T vmin, T vmax)
{
    return v < vmin ? vmin : v > vmax ? vmax : v;
}

template<typename T>
T sgn(T v)
{
  return v > T(0) ? T(1) : T(-1);
}

template<typename T>
T degToRad(T angle) {
    return angle * 3.14159265358 / T(180) ;
}

template<typename T>
T radToDeg(T angle) {
    return angle * T(180) / 3.14159265358;
}

class Timer {
public:
    Timer(int time = 0, bool resetWhenDone = true) : t0(millis()), t1(t0), time(time), resetWhenDone(resetWhenDone) {}

    operator bool() {
       return isDone();
    }

    void update() {
        t1 = millis();
    }

    int elapsed() {
        return t1 - t0;
    }

    float elapsedPercentage() {
        return clip<float>((t1 - t0) / float(time), 0, 1);
    }

    bool isDone() {
        update();
        bool done = elapsed() >= time;
        if (done && resetWhenDone) reset();
        return done;
    }

    void reset() {
        t0 = millis();
    }

    void reset(int t) {
        time = t;
        t0 = millis();
    }

    void setResetWhenDone(bool val) {
        resetWhenDone = val;
    }
private:
  int t0;
  int t1;
  int time;
  bool resetWhenDone = true;
};
