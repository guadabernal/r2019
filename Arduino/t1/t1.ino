#include "pins.h"
#include "utility.h"
#include "pidS.h"
#include "dc_motor.h"

// Motor(int pinA, int pinB, int pinPWM, int pinEnaA, int pinEnaB, int pinCS)
Motor RFR(AERFR, BERFR, PWMRFR, INARFR, INBRFR, CSRFR, SIGFR);
Motor RFL(AERFL, BERFL, PWMRFL, INARFL, INBRFL, CSRFL, SIGFL);
Motor RBR(AERBR, BERBR, PWMRBR, INARBR, INBRBR, CSRBR, SIGBR);
Motor RBL(AERBL, BERBL, PWMRBL, INARBL, INBRBL, CSRBL, SIGBL);
void updateARFR() { RFR.updateA(); }
void updateBRFR() { RFR.updateB(); }
void updateARFL() { RFL.updateA(); }
void updateBRFL() { RFL.updateB(); }
void updateARBR() { RBR.updateA(); }
void updateBRBR() { RBR.updateB(); }
void updateARBL() { RBL.updateA(); }
void updateBRBL() { RBL.updateB(); }


bool allDone() {
  return RBL.isDone() && RFL.isDone() && RBR.isDone() && RFR.isDone();
}

void setupMotors() {
  RBL.setupMotor();
  RBR.setupMotor();
  RFL.setupMotor();  
  RFR.setupMotor();  
}

void initializeMotors() {
    RBL.initialize(1, 80, 40, 500);
    RBR.initialize(0, 80, 40, 500);  
    RFL.initialize(0, 80, 40, 500);    
    RFR.initialize(1, 80, 40, 500);
}

void updateMotors() {
   RBL.update();
   RBR.update();  
   RFL.update();
   RFR.update();
}

void waitForMotors(int t) {
    while(!allDone()){
      updateMotors();
      delay(t);
    }  
}

void resetCounters() {
    RBL.resetCounter();
    RBR.resetCounter();  
    RFL.resetCounter();
    RFR.resetCounter();
}

void motorsOff() {
  RBL.off();
  RBR.off();
  RFL.off();
  RBR.off();
}

void setup() {
	  pinMode(LED_BUILTIN, OUTPUT);
    setupMotors();
  	attachInterrupt(digitalPinToInterrupt(RFR.getPinA()), updateARFR, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RFR.getPinB()), updateBRFR, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RFL.getPinA()), updateARFL, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RFL.getPinB()), updateBRFL, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RBR.getPinA()), updateARBR, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RBR.getPinB()), updateBRBR, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RBL.getPinA()), updateARBL, CHANGE);
  	attachInterrupt(digitalPinToInterrupt(RBL.getPinB()), updateBRBL, CHANGE);
    initializeMotors();
    waitForMotors(1);
    delay(500);
    resetCounters();
    RBL.goToAngle(8, 40,-40, false);
    RBR.goToAngle(-10, 40,-40, false); 
    RFL.goToAngle(-3, 40,-40, false); 
    RFR.goToAngle(8, 40,-40, false);
    
    waitForMotors(1);
    delay(500);
    resetCounters();
    RBL.goToAngle(90, 100,-100);
    RBR.goToAngle(-90, 100,-100);
    RFL.goToAngle(-90, 100,-100);
    RFR.goToAngle(90, 100,-100);    
    waitForMotors(1);
    delay(500);
    Serial.println("Counters");
    Serial.print("RBL.counter = "); Serial.println(RBL.counter);
    Serial.print("RBR.counter = "); Serial.println(RBR.counter);
    Serial.print("RFL.counter = "); Serial.println(RFL.counter);
    Serial.print("RFR.counter = "); Serial.println(RFR.counter);
    motorsOff();
}

Timer TLed(2000);
uint8_t TLedStatus = HIGH;

void loop() {
  if (TLed) { 
		digitalWrite(LED_BUILTIN, TLedStatus); 
		TLedStatus = !TLedStatus;
 	}
  delay(10);
}
