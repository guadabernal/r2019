#include "utility.h"
#include "pins.h"
#include "rmotors.h"

#define CMD_ROT_RESET_POS       30 
#define CMD_ROT_DIR_ANGLE       31
#define CMD_ROT_ROTATE          32
#define CMD_ROT_DIR_DANGLE      33


// Serial ports
#define SonarSerial Serial4
#define T0Serial    Serial1
#define Debug       Serial

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
  //  Debug.begin(115200); // Debug serial port
    T0Serial.begin(115200); // TMain communication
    Serial.begin(115200);
    motors.setup();
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::FR)), updateARFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::FR)), updateBRFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::FL)), updateARFL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::FL)), updateBRFL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::BR)), updateARBR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::BR)), updateBRBR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::BL)), updateARBL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::BL)), updateBRBL, CHANGE);
    delay(1000);
  //  motors.resetPosition();
    Serial.println("Setup Done");
}

void loop() {
  if(T0Serial.available() > 0) {
    uint8_t cmd;
    SerialRead<uint8_t>(T0Serial, &cmd);
    Serial.println(cmd);
    switch (cmd) {
      case CMD_ROT_RESET_POS: {
        Serial.println("CMD_ROT_RESET_POS");
        Debug.println("Received resetPos");
        motors.resetPosition(); // blocking here, T0 should wait untill we're done
        SerialWriteOK(T0Serial);
        break;
      }
      case CMD_ROT_DIR_ANGLE: {
        Serial.println("DirAngle");
        float angleL, angleR;
        SerialRead<float>(T0Serial, &angleL);
        SerialRead<float>(T0Serial, &angleR);
        motors.angleDir(angleL, angleR);
        break;
      }
      case CMD_ROT_DIR_DANGLE: {
        float dangle[2];
        SerialRead<float>(T0Serial, dangle, 2);
        motors.deltaAngleDir(dangle[0], dangle[1]);
        break;
      }

      case CMD_ROT_ROTATE: {
        motors.rotatePos(); // blocking here, T0 should wait untill we're done
        SerialWriteOK(T0Serial);
        break;        
      }
    }
  }
   //Debug.println("update");
  motors.update();

  delay(1);
}
