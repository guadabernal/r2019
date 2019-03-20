#include <Nextion.h>
#include "t0_comm.h"
#include "utility.h"
#include "pins.h"
#include "dc_motor.h"

T0COMM t0comm;
int robotStatus = 0;


// Motor(int pinA, int pinB, int pinPWM, int pinEnaA, int pinEnaB, int pinCS)
Motor FR(AEFR, BEFR, PWMFR, INAFR, INBFR, CSFR);
Motor FL(AEFL, BEFL, PWMFL, INAFL, INBFL, CSFL);
Motor BR(AEBR, BEBR, PWMBR, INABR, INBBR, CSBR);
Motor BL(AEBL, BEBL, PWMBL, INABL, INBBL, CSBL);

NexButton bStart = NexButton(0, 1, "bStart");
NexTouch *nex_listen_list[] = { &bStart };

void bStartPopCallback(void *ptr) { robotStatus = 1; }

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
    delay(3000);
    t0comm.led(1,T0COMM::mode_fade, T0COMM::red);

    attachInterrupt(digitalPinToInterrupt(FR.getPinA()), updateAFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(FR.getPinB()), updateBFR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(FL.getPinA()), updateAFL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(FL.getPinB()), updateBFL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BR.getPinA()), updateABR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BR.getPinB()), updateBBR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BL.getPinA()), updateABL, CHANGE);
    attachInterrupt(digitalPinToInterrupt(BL.getPinB()), updateBBL, CHANGE);
    Serial3.begin(115200);
    bStart.attachPop(bStartPopCallback);
}

Timer TLed(2000);
Timer TStrip(2000);

int TLedStatus = HIGH;
void loop() {
  if (robotStatus) {
  	if (TLed) { 
  		digitalWrite(LED_BUILTIN, TLedStatus); 
  		TLedStatus = !TLedStatus;
   	}
    if (TStrip) {
      t0comm.ledOn(0);
      t0comm.ledRGBA(0, 128,50, 240, 0);
    }
    delay(1);
  }
  else {
 	  delay(100);
  }
}
