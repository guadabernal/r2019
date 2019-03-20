#include "led_strip.h"
#include "utility.h"

LEDStrip strip0(GLED0, RLED0, BLED0, ALED0);
LEDStrip strip1(GLED1, RLED1, BLED1, ALED1);

void setup() {
    Serial.begin(9600); // Debug serial port
    Serial1.begin(115200); // TMain communication
}

Timer t1(2000);

#define CMD_LED0_RGBA    1 
#define CMD_LED0_OFF     2
#define CMD_LED1_RGBA    3
#define CMD_LED1_OFF     4

void loop() {
    if(Serial1.available() > 0){
        uint8_t cmd;
        Serial1Read<uint8_t>(&cmd);
        switch (cmd) {
            CMD_LED0_RGBA: {
                while (Serial1.available() < 4) delay(1);
                uint16_t rgba[4];
                Serial1Read<uint16_t>(rgba);
                led_strip0.rgba(rgba[0],rgba[1],rgba[2],rgba[3]);
                break;
            }
        }
        // char received[3];
        // Serial1.readBytes(received, 3);   //101
        
        // if(received[0] == 0) {
        //     if(received[1] == 0) strip1.fadeMode(static_cast<LEDStrip::Color>(received[2]));
        //     else strip1.blinkMode(static_cast<LEDStrip::Color>(received[2]));
        // }
        // else {
        //     if(received[1] == 0) strip2.fadeMode(static_cast<LEDStrip::Color>(received[2]));
        //     else strip2.blinkMode(static_cast<LEDStrip::Color>(received[2]));
        // }
    }
  
    strip0.update();
    strip1.update();
    delay(10);
}
