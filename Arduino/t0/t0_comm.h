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

#define LED_NMODES 7
uint8_t modes[LED_NMODES] = {3, 4, 5, 6, 7, 8, 9};

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
        LPSerial.begin(57600);
        CSerial.begin(57600);
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

    void ledRed(uint8_t strip){
        uint8_t send[2]  = { CMD_LED_RED, strip };
        SerialWrite<uint8_t>(T2Serial, send, 2);
    }
    void ledGreen(uint8_t strip){
        uint8_t send[2]  = { CMD_LED_GREEN, strip };
        SerialWrite<uint8_t>(T2Serial, send, 2);
    }
    void ledBlue(uint8_t strip){
        uint8_t send[2]  = { CMD_LED_BLUE, strip };
        SerialWrite<uint8_t>(T2Serial, send, 2);
    }
    void ledWhite(uint8_t strip){
        uint8_t send[2]  = { CMD_LED_WHITE, strip };
        SerialWrite<uint8_t>(T2Serial, send, 2);
    }
    void ledWhiteOnly(uint8_t strip){
        uint8_t send[2]  = { CMD_LED_WHITEONLY, strip };
        SerialWrite<uint8_t>(T2Serial, send, 2);
    }
    void ledWhiteFull(uint8_t strip){
        uint8_t send[2]  = { CMD_LED_WHITEFULL, strip };
        SerialWrite<uint8_t>(T2Serial, send, 2);
    }
    void ledDim(uint8_t strip, uint8_t dim){
        uint8_t send[3]  = { CMD_LED_DIM, strip, dim };
        SerialWrite<uint8_t>(T2Serial, send, 3);
    }
    void ledBlinkMode(uint8_t strip){
        uint8_t send[2]  = { CMD_LED_BLINKMODE, strip };
        SerialWrite<uint8_t>(T2Serial, send, 2);
    }

    void ledNextMode(uint8_t strip, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
        currentMode[strip]++;
        if (currentMode[strip] >= LED_NMODES) currentMode[strip] = 0;
        Serial.println(currentMode[strip]);
        uint8_t mode = modes[currentMode[strip]];
        switch (mode) {
            case 3: { 
                ledRGBA(strip, r, g, b, a);
                break;
            }
            case 4: { 
                ledRed(strip);
                break;
            }
            case 5: { 
                ledGreen(strip);
                break;
            }
            case 6: { 
                ledBlue(strip);
                break;
            }
            case 7: { 
                ledWhite(strip);
                break;
            }
            case 8: { 
                ledWhiteOnly(strip);
                break;
            }
            case 9: { 
                ledWhiteFull(strip);
                break;
            }
        }
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
    void rotDirAngle(float angleL, float angleR) {
        uint8_t cmd = CMD_ROT_DIR_ANGLE;
        SerialWrite<uint8_t>(T1Serial, &cmd);
        SerialWrite<float>(T1Serial, &angleL);
        SerialWrite<float>(T1Serial, &angleR);
    }
    void rotDirDAngle(float dangleL, float dangleR) {
        uint8_t cmd = CMD_ROT_DIR_DANGLE;
        SerialWrite<uint8_t>(T1Serial, &cmd);
        SerialWrite<float>(T1Serial, &dangleL);
        SerialWrite<float>(T1Serial, &dangleR);
    }
    
    //
    // set the other led modes and battery levels
    // ....

    // Controller commands
    void readControllerStatus() {
        uint8_t cmd = CMD_CONT_READ;
        SerialWrite<uint8_t>(CSerial, &cmd);
        delay(1);
        SerialRead<ControllerData>(CSerial, &controllerStatus);
    }

    ControllerData controllerStatus;  
    int currentMode[2] = {0};
protected:
            
private:
  
};
