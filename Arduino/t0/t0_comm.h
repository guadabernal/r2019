#pragma once
#include <Arduino.h>

// T2 Commands
#define CMD_LED_OFF              1 
#define CMD_LED_ON               2
#define CMD_LED_RGBA             3
#define CMD_LED_RED              4
#define CMD_LED_GREEN            5
#define CMD_LED_BLUE             6
#define CMD_LED_WHITE            7
#define CMD_LED_WHITEONLY        8
#define CMD_LED_WHITEFULL        9
#define CMD_LED_DIM             10
#define CMD_LED_BLINKMODE       11
#define CMD_LED_JUMPMODE        12
#define CMD_LED_SMOOTHMODE      13
#define CMD_LED_FADEMODE        14
#define CMD_LED_FADECURRENTMODE 15
#define CMD_BATTERY_CAPACITY    50

class T0COMM {
public:
    T0COMM() : T2Serial(Serial2) {}

    void initialize(){
        T2Serial.begin(115200);
    }

    void ledOn(uint8_t strip){
        uint8_t send[2] = { CMD_LED_ON, strip };
        SerialWrite<uint8_t>(T2Serial, send, 2);
    }

    void ledRGBA(uint8_t strip, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
        uint8_t send[6]  = { CMD_LED_RGBA, strip, r, g, b, a };
        SerialWrite<uint8_t>(T2Serial, send, 6);
    }
    // set the other led modes

protected:
            
private:
  Stream T2Serial;
};