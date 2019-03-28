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
  Serial.begin(115200);
  if (Usb.Init() == -1) {
    while(true) delay(100);
  }
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
