#include <Nextion.h>
#include "t0_comm.h"
#include "utility.h"
#include "pins.h"
#include "dc_motor.h"
#include "ui.h"
#include "pidS.h"

T0COMM comms;

// Motor(int pinA, int pinB, int pinPWM, int pinEnaA, int pinEnaB, int pinCS)
Motor FR(AEFR, BEFR, PWMFR, INAFR, INBFR, CSFR);
Motor FL(AEFL, BEFL, PWMFL, INAFL, INBFL, CSFL);
Motor BR(AEBR, BEBR, PWMBR, INABR, INBBR, CSBR);
Motor BL(AEBL, BEBL, PWMBL, INABL, INBBL, CSBL);

void updateAFR() { FR.updateA(); }
void updateBFR() { FR.updateB(); }
void updateAFL() { FL.updateA(); }
void updateBFL() { FL.updateB(); }
void updateABR() { BR.updateA(); }
void updateBBR() { BR.updateB(); }
void updateABL() { BL.updateA(); }
void updateBBL() { BL.updateB(); }

void setup() { 
  	pinMode(LED_BUILTIN, OUTPUT);

    comms.initialize();
    BL.setupMotor();
    BR.setupMotor();
    FR.setupMotor();
    FL.setupMotor();
    
    attachInterrupt(digitalPinToInterrupt(FR.getPinA()), updateAFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(FR.getPinB()), updateBFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(FL.getPinA()), updateAFL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(FL.getPinB()), updateBFL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BR.getPinA()), updateABR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BR.getPinB()), updateBBR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BL.getPinA()), updateABL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BL.getPinB()), updateBBL, CHANGE);

    UIInitialize();
    
    delay(500);
    comms.ledOn(0);
    comms.ledRGBA(0, 30, 150, 240, 0);
    delay(500);
    comms.ledOff(0);
    Debug.println("setup done");
    delay(2000);
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
        Serial.println(dAngle);
        comms.rotDirDAngle(dAngle);
      }
      int rightHatY = int(comms.controllerStatus.rightHatY) - 127;
      if (abs(rightHatY) < 9) rightHatY = 0;
      int dir = 1;
      float vel = abs(rightHatY) / 127.0f * 50;
      Serial.println(vel);
      if (rightHatY < 0) dir = 0;
      BL.setDir(1 - dir);
      BL.setPWM(vel);
      FL.setDir(1 - dir);
      FL.setPWM(vel);   
      BR.setDir(dir);
      BR.setPWM(vel);        
      FR.setDir(dir);
      FR.setPWM(vel);
    }
  }

//   nexLoop(nex_listen_list);
  delay(1);
}
