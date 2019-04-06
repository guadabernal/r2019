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
    comms.ledRGBA(0, 255, 0, 0, 0);
    comms.ledRGBA(1, 255, 0, 0, 0);
    //comms.ledRGBA(0, 30, 150, 240, 0);
    //comms.ledRGBA(1, 240, 0, 240, 0);
    delay(500);
   // comms.ledOff(0);
    Debug.println("setup done");
    delay(2000);
    //comms.rotResetPos();
    //motors.goToDistance(20, -0.2);

//    delay(1000);
//    comms.rotResetPos(); // blocking
//    robot.setAngle(0);
//    robot.setMode(Robot::Normal);
//    delay(1000);
//    comms.rotRotate(); // blocking
//    robot.setMode(Robot::Rotate);
//    motors.goToSpeed(0, 0);
//    delay(1000);    
    motors.rotateToDistance(20, 0.42);
    motors.waitForAll();
    delay(1000);
    motors.rotateToDistance(20, 0.42);
    motors.waitForAll();
    delay(1000);
    motors.rotateToDistance(20, 0.42);
    motors.waitForAll();
    delay(1000);
    motors.rotateToDistance(20, 0.42);
    motors.waitForAll();
    delay(1000);
}

Timer TLed(2000);
Timer ledChange(300);
Timer ledDim(50);
int TLedStatus = HIGH;

Timer TController(5);
Timer TSController(30);

Timer TT1(2000);
int t1done = false;
int dim = 50;

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
//  nexLoop(nex_listen_list);
  delay(1);
}
