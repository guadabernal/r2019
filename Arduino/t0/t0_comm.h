#pragma once
#include <Arduino.h>
#include "utility.h"
#include "controller.h"

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
#define CMD_BATTERY_CAPACITY    16

// T1 Commands
#define CMD_ROT_RESET_POS       30 
#define CMD_ROT_DIR_ANGLE       31
#define CMD_ROT_ROTATE          32
#define CMD_ROT_DIR_DANGLE      33


// Controller Commands
#define CMD_CONT_READ           40

#define Debug                   Serial
#define LPSerial                Serial4
#define T2Serial                Serial2
#define T1Serial                Serial1
#define DisplaySerial           Serial3 // hardcoded in nextion library, because they are lazy
#define CSerial                 Serial6 // green RX blue TX at t0

class T0COMM {
public:
    T0COMM() {}

    void initialize(){
        Debug.begin(9600);
        T1Serial.begin(115200);
        T2Serial.begin(115200);
        DisplaySerial.begin(115200);
        CSerial.begin(115200);
    }

    // T2 Commands
    void ledOn(uint8_t strip){
        uint8_t send[2] = { CMD_LED_ON, strip };
        SerialWrite<uint8_t>(T2Serial, send, 2);
    }

    void ledOff(uint8_t strip){
        uint8_t send[2] = { CMD_LED_OFF, strip };
        SerialWrite<uint8_t>(T2Serial, send, 2);
    }
    
    void ledJumpMode(uint8_t strip, uint8_t set, uint16_t time) {
        uint8_t send[3] = { CMD_LED_JUMPMODE, strip , set};
        SerialWrite<uint8_t>(T2Serial, send, 3);
        SerialWrite<uint16_t>(T2Serial, &time);
    }

    void ledRGBA(uint8_t strip, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
        uint8_t send[6]  = { CMD_LED_RGBA, strip, r, g, b, a };
        SerialWrite<uint8_t>(T2Serial, send, 6);
    }
    //
    // set the other led modes and battery levels
    // ....

    // T1 Commands
    void rotResetPos() {
        uint8_t cmd = CMD_ROT_RESET_POS;
        SerialWrite<uint8_t>(T1Serial, &cmd);
        SerialReadOK(T1Serial); // blocking
    }
    void rotRotate() {
        uint8_t cmd = CMD_ROT_ROTATE;
        SerialWrite<uint8_t>(T1Serial, &cmd);
        SerialReadOK(T1Serial); // blocking
    }
    void rotDirAngle(float angle) {
        uint8_t cmd = CMD_ROT_DIR_ANGLE;
        SerialWrite<uint8_t>(T1Serial, &cmd);
        SerialWrite<float>(T1Serial, &angle);
    }
    void rotDirDAngle(float dangle) {
        uint8_t cmd = CMD_ROT_DIR_DANGLE;
        SerialWrite<uint8_t>(T1Serial, &cmd);
        SerialWrite<float>(T1Serial, &dangle);      
    }
    //
    // set the other led modes and battery levels
    // ....

    // Controller commands
    void readControllerStatus() {
        uint8_t cmd = CMD_CONT_READ;
        SerialWrite<uint8_t>(CSerial, &cmd);
        SerialRead<ControllerData>(CSerial, &controllerStatus);
    }

    ControllerData controllerStatus;  
protected:
            
private:
  
};
