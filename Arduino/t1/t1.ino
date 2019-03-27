#include "utility.h"
#include "pins.h"
#include "rmotors.h"

#define T0Serial Serial1
#define Debug    Serial

#define CMD_ROT_RESET_POS            1 
#define CMD_ROT_DIR_ANGLE            2
#define CMD_ROT_ROTATE               3

RMotors motors;

void updateARFR() { motors.updateA(RMotors::FR); }
void updateBRFR() { motors.updateB(RMotors::FR); }
void updateARFL() { motors.updateA(RMotors::FL); }
void updateBRFL() { motors.updateB(RMotors::FL); }
void updateARBR() { motors.updateA(RMotors::BR); }
void updateBRBR() { motors.updateB(RMotors::BR); }
void updateARBL() { motors.updateA(RMotors::BL); }
void updateBRBL() { motors.updateB(RMotors::BL); }


void setup() {
    Debug.begin(9600); // Debug serial port
    T0Serial.begin(115200); // TMain communication

    pinMode(LED_BUILTIN, OUTPUT);
    setupMotors();
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::FR)), updateARFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::FR)), updateBRFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::FL)), updateARFL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::FL)), updateBRFL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::BR)), updateARBR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::BR)), updateBRBR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::BL)), updateARBL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::BL)), updateBRBL, CHANGE);

    motors.resetPosition();
}

Timer TLed(2000);
uint8_t TLedStatus = HIGH;

void loop() {
  if (TLed) { 
		digitalWrite(LED_BUILTIN, TLedStatus); 
		TLedStatus = !TLedStatus;
 	}
  if(T0Serial.available() > 0) {
    uint8_t cmd;
    SerialRead<uint8_t>(T0Serial, &cmd);
    switch (cmd) {
      case CMD_ROT_RESET_POS: {
        motors.resetPosition(); // blocking here, T0 should wait untill we're done
        SerialWriteOK(T0Serial);
        break;
      }
      case CMD_ROT_DIR_ANGLE: {
        float angle;
        SerialRead<float>(T0Serial, &angle);
        motors.angleDir(angle);
        break;
      }
      case CMD_ROT_ROTATE: {
        motors.rotatePos(); // blocking here, T0 should wait untill we're done
        SerialWriteOK(T0Serial);
        break;        
      }
    }
  }

  motors.update();

  delay(1);
}
