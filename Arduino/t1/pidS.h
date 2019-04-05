#pragma once
#include <Arduino.h>


template<typename T, typename K>
class PID
{
public:
    PID() {}

    PID(K Kp, K Ki, K Kd, K maxOutput = 1023, K minOutput = -1023) 
    : Kp(Kp), Ki(Ki), Kd(Kd), maxOutput(maxOutput), minOutput(minOutput)
    {}

    void setTarget(T tgt, K inmaxOutput = 1023, K inminOutput = -1023) {
        maxOutput = inmaxOutput;
        minOutput = inminOutput;
        target = tgt;
        t0 = micros();
    }
 
    K compute(T input) {
        t1 = micros();
        K dt = (t1 - t0) / 1E6;
        t0 = t1;
        if (dt < 1E-5) return output0;

        err = target - input;
        iErr += err * dt;
        dErr = (err - err0) / dt;
        err0 = err;
        K output = K(Kp * err + Ki * iErr + Kd * dErr);
        output = output > maxOutput ? maxOutput : output < minOutput ? minOutput : output;
        output0 = output;
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
    K output0 = 0;

    long t0 = 0;
    long t1 = 0;
};
