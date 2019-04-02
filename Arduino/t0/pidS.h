#pragma once
#include <Arduino.h>


template<typename T, typename K>
class PID
{
public:
    PID() {}

    PID(K Kp, K Ki, K Kd) : Kp(Kp), Ki(Ki), Kd(Kd) {}

    void reset(T tgt, K inmaxOutput, K inminOutput) {
        maxOutput = inmaxOutput;
        minOutput = inminOutput;
        target = tgt;
        iErr = 0;
        err0 = 0;
        t0 = micros();
    }
  
    K compute(T input) {
        t1 = micros();
        K dt = (K(t1) - K(t0)) * 1E-6;
        t0 = t1;
        if (dt == 0) return 0;

        err = target - input;   
        if (Ki != 0)
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
