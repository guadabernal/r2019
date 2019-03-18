#include "led_strip.h"

LEDStrip strip1(22, 23, 21, 20);
LEDStrip strip2(5, 6, 4, 3);

void setup() {
    //strip1.fadeMode(LEDStrip::red);
    //strip1.blinkMode(LEDStrip::green, 200);
    strip1.fadeMode(LEDStrip::red);
    //strip2.blinkMode(LEDStrip::green, 200);
    Serial.begin(9600);
    Serial1.begin(115200);
}
int t0 = millis();
void loop() {
    if(Serial1.available() > 0){
        char received[3];
        Serial1.readBytes(received, 3);   //101
        
        if(received[0] == 1) {
            Serial.print("received[0] = 1");
            if(received[1] == 1) strip1.fadeMode(static_cast<LEDStrip::Color>(received[2]));
            else strip1.blinkMode(static_cast<LEDStrip::Color>(received[2]));
        }
        else {
            Serial.print("received[0] = 0");
            if(received[1] == 1) strip2.fadeMode(static_cast<LEDStrip::Color>(received[2]));
            else strip2.blinkMode(static_cast<LEDStrip::Color>(received[2]));
        }
        
        Serial.print("Received   ");
        Serial.print((int)received[0]);
        Serial.print((int)received[1]);
        Serial.println((int)received[2]);
    }
    int t1 = millis();
    if (t1 - t0 > 2000) {
        Serial.println("T2 Ok");
        t0 = t1;
    }
  
    strip1.update();
    delay(100);
}
