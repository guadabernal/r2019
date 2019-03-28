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

Timer TController(30);

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
      Debug.print("Connected = ");
      Debug.print(comms.controllerStatus.connected);
      Debug.print("Triangle = ");
      Debug.println(comms.controllerStatus.triangle);
      if (comms.controllerStatus.triangle) {
        comms.rotResetPos(); // blocking
        Debug.println("sending reset pos");
      }
      if (comms.controllerStatus.circle) {
        comms.rotRotate(); // blocking
        Debug.println("sending reset pos");
      }
      if (comms.controllerStatus.cross) {
        comms.rotDirAngle(30); // blocking
        Debug.println("sending reset pos");
      }
    }
  }

//   nexLoop(nex_listen_list);
  delay(1);
}
