#pragma once
#include <Arduino.h>

class T0COMM {
public:
    enum Mode {mode_fade, mode_blink};
    enum Color { red, green, blue, white};

    void initiallize(){
        Serial2.begin(115200);
    }

    void led(int strip, Color color, Mode mode){
        char send[3];
        send[0] = strip;    //1
        send[1] = color;    //0
        send[2] = mode;     //1s
        Serial2.write(send, 3);
    }

protected:
            
private:
    
};
