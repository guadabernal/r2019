#include "pins.h"
#include "utility.h"
#include "pidS.h"
#include "dc_motor.h"

// Motor(int pinA, int pinB, int pinPWM, int pinEnaA, int pinEnaB, int pinCS)
Motor RFR(AERFR, BERFR, PWMRFR, INARFR, INBRFR, CSRFR);
Motor RFL(AERFL, BERFL, PWMRFL, INARFL, INBRFL, CSRFL);
Motor RBR(AERBR, BERBR, PWMRBR, INARBR, INBRBR, CSRBR);
Motor RBL(AERBL, BERBL, PWMRBL, INARBL, INBRBL, CSRBL, SIGBL);
void updateARFR() { RFR.updateA(); }
void updateBRFR() { RFR.updateB(); }
void updateARFL() { RFL.updateA(); }
void updateBRFL() { RFL.updateB(); }
void updateARBR() { RBR.updateA(); }
void updateBRBR() { RBR.updateB(); }
void updateARBL() { RBL.updateA(); }
void updateBRBL() { RBL.updateB(); }



void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
  	RBL.setupMotor();
    
  	attachInterrupt(digitalPinToInterrupt(RFR.getPinA()), updateARFR, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RFR.getPinB()), updateBRFR, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RFL.getPinA()), updateARFL, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RFL.getPinB()), updateBRFL, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RBR.getPinA()), updateARBR, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RBR.getPinB()), updateBRBR, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RBL.getPinA()), updateARBL, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RBL.getPinB()), updateBRBL, CHANGE);

  	RBL.initialize(0, 50, 40, 500);
    //RBR.initialize(0, 40, 20, 1500);
    //RFL.initialize(0, 40, 20, 1500);
    //RFR.initialize(0, 40, 20, 1500);
    while(!RBL.isDone()){     //|| !RBR.isInit || !RFL.isInit || !RFR.isInit
      RBL.update();
      //RBR.update();
      //RFL.update();
      //RFR.update();
      delay(1);
    }
    delay(500);
    RBL.resetCounter();
    RBL.goToAngle(-90, 50,-50 );
    while(!RBL.isDone()) {
      RBL.update();
      delay(1);
    }
    //RBL.off();
}

Timer TLed(2000);
uint8_t TLedStatus = HIGH;

void loop() {
  if (TLed) { 
		digitalWrite(LED_BUILTIN, TLedStatus); 
		TLedStatus = !TLedStatus;
 	}
 	RBL.update();
  delay(1);
}
