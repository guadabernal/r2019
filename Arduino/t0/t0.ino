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
    comms.ledRGBA(0, 30, 150, 240, 0);
    delay(500);
    comms.ledOff(0);
    Debug.println("setup done");
    delay(2000);

    motors.goToSpeed(RMotors::BR, 50);
}

Timer TLed(2000);
int TLedStatus = HIGH;

Timer TController(10);

Timer TT1(2000);
int t1done = false;

void loop() {
  if (TLed) { 
    digitalWrite(LED_BUILTIN, TLedStatus); 
    TLedStatus = !TLedStatus;  
  }
  if (TT1 && !t1done) {
    Debug.println("sending reset pos");
    t1done = true;
    //comms.rotResetPos(); // blocking
  }
  if (TController) {
    comms.readControllerStatus(); // blocking
    if (comms.controllerStatus.connected) {
      if (comms.controllerStatus.triangle) {
        comms.rotResetPos(); // blocking
        Debug.println("sending reset pos");
      }
      if (comms.controllerStatus.circle) {
        comms.rotRotate(); // blocking
        Debug.println("sending reset pos");
      }
      if (comms.controllerStatus.cross) {
        comms.rotDirAngle(0); // blocking
        Debug.println("sending reset pos");
      }
      int leftHatX = int(comms.controllerStatus.leftHatX)- 127;
      if (abs(leftHatX) < 9) leftHatX = 0;
      if (abs(leftHatX) > 100) {
        float dAngle = 3 * sgn(leftHatX);
        robot.update(dAngle);
        comms.rotDirDAngle(robot.dl, robot.dr);
      }
      int rightHatY = int(comms.controllerStatus.rightHatY) - 127;
      if (abs(rightHatY) < 9) rightHatY = 0;
      int dir = 1;
      float vel = abs(rightHatY) / 127.0f * 50;
      Serial.println(vel);
      if (rightHatY < 0) dir = 0;
      // BL.setDir(1 - dir);
      // BL.setPWM(vel);
      // FL.setDir(1 - dir);
      // FL.setPWM(vel);   
      // BR.setDir(dir);
      // BR.setPWM(vel);        
      // FR.setDir(dir);
      // FR.setPWM(vel);
    }
  }
  motors.update();
//   nexLoop(nex_listen_list);
  delay(1);
}
