#pragma once
#include <Arduino.h>

template<typename T, typename K>
class PID
{
public:
    PID() {}

    PID(K Kp, K Ki, K Kd, K maxOutput, K minOutput) : Kp(Kp), Ki(Ki), Kd(Kd), maxOutput(maxOutput), minOutput(minOutput) {}


    K compute(T input) {
        t1 = micros();
        K dt = (K(t1) - K(t0)) * 1E-6;
        if (dt < 1E-4) return output0;                      
        t0 = t1;
        err = target - input;   
        if (Ki != 0)
          iErr += err * dt;
        dErr = (err - err0) / dt;
        err0 = err;
        K output = K(Kp * err + Ki * iErr + Kd * dErr);
        output = output > maxOutput ? maxOutput : output < minOutput ? minOutput : output;
        output0 = output;
        return output;
    }
  
    T getTarget() { return target; }
    void setTarget(T tgt) {
       t0 = micros();
       target = tgt;
    }

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
