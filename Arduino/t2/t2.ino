#include "led_strip.h"
#include "utility.h"
#include "pins.h"

LEDStrip::LEDStrip strip[2];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600); // Debug serial port
    Serial1.begin(115200); // TMain communication
    strip[0].setPins(GLED0, RLED0, BLED0, ALED0);
    strip[1].setPins(GLED1, RLED1, BLED1, ALED1);
}

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
#define CMD_LED_OFFMODE         16
#define CMD_BATTERY_CAPACITY    50

Timer TLed(2000);
uint8_t TLedStatus = HIGH;
void loop() {
  if (TLed) { 
    digitalWrite(LED_BUILTIN, TLedStatus); 
    TLedStatus = !TLedStatus;
  }
    if(Serial1.available() > 0){
        uint8_t cmd;
        Serial1Read<uint8_t>(&cmd);
        switch (cmd) {
            case CMD_LED_ON: {
                uint8_t index;
                Serial1Read<uint8_t>(&index);
                if (index < 2) strip[index].on();
                break;
            }
            case CMD_LED_OFF: {
                uint8_t index;
                Serial1Read<uint8_t>(&index);
                if (index < 2) strip[index].off();
                break;
            }
            case CMD_LED_RGBA: {
                uint8_t index;
                uint8_t rgba[4];
                Serial1Read<uint8_t>(&index);
                Serial1Read<uint8_t>(rgba, 4);
                if (index < 2) strip[index].rgba(rgba[0],rgba[1],rgba[2],rgba[3]);
                break;
            }
            case CMD_LED_JUMPMODE: {
                uint8_t index;
                uint8_t set;
                uint16_t time;
                Serial1Read<uint8_t>(&index);
                Serial1Read<uint8_t>(&set);
                Serial1Read<uint16_t>(&time);
                if (index < 2) strip[index].jumpMode(set, time);
                break;
            }        
            // .. describe other commands
        }
    }
  
    strip[0].update();
    strip[1].update();
    delay(10);
}
