#include "T0COMM.h"
#include "utility.h"
#include "pins.h"
#include "dc_motor.h"

T0COMM t0comm;

//Motor(int pinA, int pinB, int pinPWM, int pinEnaA, int pinEnaB, int pinCS)
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

    t0comm.initiallize();
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
}

Timer TLed(2000);
Timer TStrip(2000);

int TLedStatus = HIGH;
void loop() {
	if (TLed) { 
		digitalWrite(LED_BUILTIN, TLedStatus); 
		TLedStatus = !TLedStatus;
 	}
  if (TStrip) {
    t0comm.led(0, T0COMM::mode_fade, (T0COMM::Color)random(0, 4));
  }
 	delay(1);
}
