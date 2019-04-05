#include <PS4BT.h>
#include <usbhub.h>
#include <SPI.h>
#include "controller.h"
#include "utility.h"

#define CMD_CONT_READ           40

USB Usb;
BTD Btd(&Usb);

//PS4BT PS4(&Btd, PAIR);
PS4BT PS4(&Btd);

void setup() {
  Serial.begin(57600);
  if (Usb.Init() == -1) {
    while(true) delay(100);
  }
  delay(1000);
}


ControllerData data;

void loop() {
  Usb.Task();  
  if(Serial.available() > 0) {
      uint8_t cmd;
      SerialRead<uint8_t>(Serial, &cmd);
      switch (cmd) {
        case CMD_CONT_READ: {
          data.update(PS4);
          SerialWrite<ControllerData>(Serial, &data);
          break;
        }
      }
  }
  if (PS4.getButtonPress(PS)) {
    PS4.disconnect();
  }
  delay(1);
}


//
//#include "utility.h"
//void setup() {
//  Serial.begin(57600);
//    pinMode(LED_BUILTIN, OUTPUT);
//    digitalWrite(LED_BUILTIN, HIGH);
//    delay(1000);
//    digitalWrite(LED_BUILTIN, LOW);
//    delay(1000);
//}
//
//
////#define CMD_CONT_READ           40
////bool TLedStatus = LOW;
////void loop() {
////   if(Serial.available() > 0) {   
////      uint8_t cmd;
////      SerialRead<uint8_t>(Serial, &cmd);
////      switch (cmd) {
////        case CMD_CONT_READ: {
////          digitalWrite(LED_BUILTIN, HIGH);  
////          uint8_t data[16];
////          SerialWrite<uint8_t>(Serial, data, 16);
////          delay(2000);
////          break;
////        }
////      }
////  }
////  delay(10);
////}
