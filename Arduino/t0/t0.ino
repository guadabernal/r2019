#include <Nextion.h>
#include "t0_comm.h"
#include "utility.h"
#include "pins.h"
#include "dc_motor.h"
#include "ui.h"
#include "pidS.h"

T0COMM::T0COMM t0comm;

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

    t0comm.initialize();
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
    t0comm.ledOn(0);
    t0comm.ledRGBA(0, 30, 150, 240, 0);
    delay(500);
    t0comm.ledOff(0);
    DebugSerial.print("setup done");

}

Timer TLed(2000);
Timer TM(2000);
Timer TStrip(2000);


int TLedStatus = HIGH;
long t0 = 0;
bool notset = true;
float w0 = 0;
long tinit=0;
bool motorTest = false;

void loop() {
  if (TLed) { 
    digitalWrite(LED_BUILTIN, TLedStatus); 
    TLedStatus = !TLedStatus;
  }
  if (TM && !motorTest) {
    BL.goToAngle(-3600, 100, -100);
    motorTest = true;
  }
  BL.update();
  Serial.println(BL.counter);
//  // DebugSerial.print("BL Counter = ");
//  //DebugSerial.println(BL.counter); 

//   if (robotEnable) {
//   	if (TLed) { 
//   		digitalWrite(LED_BUILTIN, TLedStatus); 
//   		TLedStatus = !TLedStatus;
//    	}
// //    if (TStrip) {
// //      t0comm.ledOn(0);
// //      t0comm.ledJumpMode(0, random(0, 4), 300);
// //    }
//     if (notset) { 
//       t0 = micros();
//       notset = false;
//       tinit= micros();
//     }
//     long t1 = micros() - t0;
//     if (t1 < 1500000) {

//       float pw = pid.compute(BL.counter);
//       if (pw < 0)
//         BL.dir(1); // right negative
//       else
//         BL.dir(0); // left positive

        
//       float dw = pw - w0;
// //      Serial.print("w0 = ");
// //      Serial.print(w0);     
// //      Serial.print("  Pw = ");
// //      Serial.print(pw);     
// //      Serial.print("  dw = ");
// //      Serial.print(dw);     
//       //if (abs(dw) > 15) pw = w0 + 15 * sgn(dw);
// //      Serial.print("  PwF = ");
// //      Serial.print(pw);           
// //      Serial.print(" counter = ");
      
//       Serial.print(long((micros() - tinit) / 1E3));
//       Serial.print(" ");
//       Serial.print(pid.getTarget());
//       Serial.print(" ");
//       Serial.println(BL.counter);
//       w0 = pw;
//       BL.speed(abs(pw));
//     }
//     else {
//       BL.stop();
//       if (pid.getTarget() == 0) pid.reset(1600); 
//       else if(pid.getTarget() == 1600) pid.reset(0);
//       delay(1000);
//       t0 = micros();
//     }

//     delay(5);
//   }
//   else {
//     BL.off();

//     t0comm.ledOff(0);
//  	  delay(10);
//   }
//   nexLoop(nex_listen_list);
}
