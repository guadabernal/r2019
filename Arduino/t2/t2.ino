#include "led_strip.h"
#include "utility.h"

LEDStrip strip1(22, 23, 21, 20);
LEDStrip strip2(5, 6, 4, 3);

void setup() {
    strip1.fadeMode(LEDStrip::blue);
    Serial.begin(9600);
    Serial1.begin(115200);
}

Timer t1(2000);

void loop() {
    if(Serial1.available() > 0){
        char received[3];
        Serial1.readBytes(received, 3);   //101
        
        if(received[0] == 0) {
            if(received[1] == 0) strip1.fadeMode(static_cast<LEDStrip::Color>(received[2]));
            else strip1.blinkMode(static_cast<LEDStrip::Color>(received[2]));
        }
        else {
            if(received[1] == 0) strip2.fadeMode(static_cast<LEDStrip::Color>(received[2]));
            else strip2.blinkMode(static_cast<LEDStrip::Color>(received[2]));
        }
    }
    if (t1) {
        Serial.println("T2 Ok");
    }
  
    strip1.update();
    delay(100);
}
