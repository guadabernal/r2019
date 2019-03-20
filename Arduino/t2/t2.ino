#include "led_strip.h"
#include "utility.h"

LEDStrip::LEDStrip strip[2];

void setup() {
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
#define CMD_BATTERY_CAPACITY    50


void loop() {
    if(Serial1.available() > 0){
        uint8_t cmd;
        Serial1Read<uint8_t>(&cmd)
        switch (cmd) {
            CMD_LED_ON: {
                uint8_t index;
                Serial1Read<uint8_t>(&index);
                if (index < 2) led_strip[index].on();
                break;
            }
            CMD_LED_RGBA: {
                uint8_t idx;
                uint8_t rgba[4];
                Serial1Read<uint8_t>(&index);
                Serial1Read<uint8_t>(rgba, 4);
                if (index < 2) led_strip[index].rgba(rgba[0],rgba[1],rgba[2],rgba[3]);
                break;
            }
            // .. describe other commands
        }
    }
  
    strip[0].update();
    strip[1].update();
    delay(10);
}
