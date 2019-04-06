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
        Serial.print("T = "); Serial.print(dt);
        if (dt < 1E-4) return output0;                      
        t0 = t1;
        err = target - input;   
        K newIErr = iErr + err * dt;
        if (Ki * newIErr < maxOutput  && Ki * newIErr > minOutput)
          iErr = newIErr;
        dErr = (err - err0) / dt;
        err0 = err;
        K output = K(Kp * err + Ki * iErr + Kd * dErr);
        output = output > maxOutput ? maxOutput : output < minOutput ? minOutput : output;
//        Serial.print(" Target = "); Serial.print(target);
//        Serial.print(" Input = "); Serial.print(input);        
//        Serial.print(" Err = "); Serial.print(err);
//        Serial.print(" maxOutput = "); Serial.print(maxOutput);
//        Serial.print(" minOutputr = "); Serial.print(minOutput);
//        Serial.print(" output = "); Serial.println(output);
        output0 = output;
        return output;
    }
  
    T getTarget() { return target; }

    
    void reset() {
      err0 = 0;
      iErr = 0;
      t0 = micros();
    }
    
    void setTarget(T tgt, bool resetTimer = true) {
       if (resetTimer)
         t0 = micros();
       target = tgt;
    }    

private:
    T target = 0;
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
