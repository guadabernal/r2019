#pragma once
#include <Arduino.h>


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
        return clip((t1 - t0) / float(time), 0, 1);
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

template<typename T>
void Serial1Read<T>(T* p, uint16_t count = 1) {
    while(Serial1.available() < count * sizeof(T)) delay(1);
    Serial1.readBytes((uint8_t*) p, count * sizeof(T));
}

template<typename T>
void Serial1Write<T>(T* p, uint16_t count = 1) {
    Serial1.write((uint8_t*) p, count * sizeof(T));
}

template<typename T>
T clip<T>(T v, t vmin, T vmax)
{
    return v < vmin ? vmin : v > vmax ? vmax : v;
}
