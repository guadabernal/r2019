#include <Nextion.h>
#include "t0_comm.h"
#include "utility.h"
#include "pins.h"
#include "dc_motor.h"
#include "ui.h"
#include "pidS.h"
#include "rmotors.h"

T0COMM comms;
RMotors motors;
Robot robot;

void updateAFR() { motors.updateA(RMotors::FR); }
void updateBFR() { motors.updateB(RMotors::FR); }
void updateAFL() { motors.updateA(RMotors::FL); }
void updateBFL() { motors.updateB(RMotors::FL); }
void updateABR() { motors.updateA(RMotors::BR); }
void updateBBR() { motors.updateB(RMotors::BR); }
void updateABL() { motors.updateA(RMotors::BL); }
void updateBBL() { motors.updateB(RMotors::BL); }


void setup() { 
  	pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH); 
    comms.initialize();
    motors.setup();
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::FR)), updateAFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::FR)), updateBFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::FL)), updateAFL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::FL)), updateBFL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::BR)), updateABR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::BR)), updateBBR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinA(RMotors::BL)), updateABL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(motors.getPinB(RMotors::BL)), updateBBL, CHANGE);

    UIInitialize();
    
    delay(500);
    comms.ledOn(0);
    comms.ledOn(1);
    comms.ledRGBA(0, 255, 0, 0, 0);
    comms.ledRGBA(1, 0, 255, 0, 0);
    delay(500);
    comms.ledOff(0);
    comms.ledOff(1);
    Debug.println("setup done");
    delay(2000);
    //comms.rotResetPos();
}

Timer TLed(2000);
Timer ledChange(300);
Timer ledDim(50);
int TLedStatus = HIGH;

Timer TController(5);
Timer TSController(30);

Timer TT1(2000);

Timer TLaptop(10);
int t1done = false;
int dim = 50;
bool ledOn = false;


void loop() {
  if (robotEnable) {
    if (!ledOn) {
      ledOn = true;
      comms.ledRGBA(1, 0, 128, 128, 0);
      comms.ledFadeCurrentMode(1, 4000);
      comms.ledOn(1);
    }
    if (TLed) { 
      digitalWrite(LED_BUILTIN, TLedStatus); 
      TLedStatus = !TLedStatus;
     // LPSerial.println("HEre");
    }
    if (TT1 && !t1done) {
      Debug.println("sending reset pos");
      t1done = true;
      //comms.rotResetPos(); // blocking
    }
//    if (TLaptop) {
//      if (Serial.available() > 0) {
//         comms.ledSmoothMode(1, 0);
//        uint8_t cmd;
//        SerialRead0<uint8_t>(&cmd);
//        switch(cmd) {
//          case CMD_LAP_STOP: {
//            motors.off();
//            break;
//          }
//          case CMD_LAP_FWD: {
//            comms.ledJumpMode(1, 0);
//            int16_t d;
//            SerialRead0<int16_t>(&d);
//            float distance = d / 32768.0 * 100;
//            comms.rotDirAngle(0, 0); // blocking
//            robot.setAngle(0);
//            robot.setMode(Robot::Normal);
//            //motors.goToDistance(20, distance);      
//            break;
//          }
//          case CMD_LAP_ROT: {
//            comms.ledFadeMode(1, 0);
//            int16_t a;
//            SerialRead0<int16_t>(&a);
//            float angle = a / 32768.0 * 180;
//            robot.increaseAngle(2 * sgn(a));
//            comms.rotDirAngle(robot.dl, robot.dr);    
//            break;
//          }
//        }
//      } 
//    }
    if (TController) {
      comms.readControllerStatus(); // blocking
      if (comms.controllerStatus.connected) {
        if (comms.controllerStatus.triangle) {
          comms.ledRGBA(0, 0, 255, 0, 0);
          comms.ledRGBA(1, 0, 255, 0, 0);
          comms.rotResetPos(); // blocking
          robot.setAngle(0);
          robot.setMode(Robot::Normal);
        }

        if (comms.controllerStatus.square && ledChange) { 
          comms.ledNextMode(1, 0, 0, 255, 0);
          comms.ledNextMode(0, 0, 0, 255, 0);
        }
        if (comms.controllerStatus.left && ledChange) { 
          comms.ledBlinkMode(0);
          comms.ledBlinkMode(1);
        }
        if (comms.controllerStatus.up && ledDim) {
          dim += 3;
          if (dim > 255) dim = 255;
          comms.ledDim(1, dim);
          comms.ledDim(0, dim);
        }
        if (comms.controllerStatus.down && ledDim) {
          dim -= 3;
          if (dim < 0) dim = 0;
          comms.ledDim(1, dim);
          comms.ledDim(0, dim);
        }


        if (comms.controllerStatus.circle) {
          comms.rotRotate(); // blocking
          robot.setMode(Robot::Rotate);
          motors.goToSpeed(0, 0);
        }
        if (comms.controllerStatus.cross) {
          comms.rotDirAngle(0, 0); // blocking
          robot.setAngle(0);
          robot.setMode(Robot::Normal);
        }
        int leftHatX = int(comms.controllerStatus.leftHatX)- 127;
        if (abs(leftHatX) < 9) leftHatX = 0;
        if (abs(leftHatX) > 100) {
          float dAngle = 0.5 * sgn(leftHatX);
          robot.increaseAngle(dAngle);
          comms.rotDirAngle(robot.dl, robot.dr);
        }
        if (TSController) {
          int rightHatY = 127-int(comms.controllerStatus.rightHatY);
          if (abs(rightHatY) < 9) rightHatY = 0;
          int dir = 1;
          robot.setVel(rightHatY / 127.0f * 40);
          motors.goToSpeed(robot.VL, robot.VR);
        }
      }
    }
    motors.update();
  } else {
    if (ledOn) {
      comms.ledOff(0);
      comms.ledOff(1);
      motors.off();
      ledOn = false;
    }
  }
  nexLoop(nex_listen_list);
  delay(1);
}
