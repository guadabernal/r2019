#pragma once
#include <Arduino.h>


template<typename T, typename K>
class PID
{
public:
    PID() {}

    PID(K Kp, K Ki, K Kd) : Kp(Kp), Ki(Ki), Kd(Kd) {}

    void reset(T tgt, K inmaxOutput = K(1023), K inminOutput = K(-1023)) {
        maxOutput = inmaxOutput;
        minOutput = inminOutput;
        target = tgt;
        iErr = 0;
        err0 = 0;
    }

    void reset(T tgt) {
        target = tgt;
        iErr = 0;
        err0 = 0;      
    }
  
    K compute(T input) {
        t1 = micros();
        K dt = (t1 - t0) / 1E6;
        t0 = t1;
        if (dt == 0) return 0;

        err = target - input;
        iErr += err * dt;
        dErr = (err - err0) / dt;
        err0 = err;
        K output = K(Kp * err + Ki * iErr + Kd * dErr);
        output = output > maxOutput ? maxOutput : output < minOutput ? minOutput : output;
        return output;
    }
  
    T getTarget() { return target; }
private:
    T target;
    K Kp; // proportional
    K Ki; // integral
    K Kd; // derivative
    K err = 0;
    K err0 = 0;
    K iErr = 0;
    K dErr = 0;
    K maxOutput;
    K minOutput;

    long t0 = 0;
    long t1 = 0;
};
