#include "T0COMM.h"

int led =   LED_BUILTIN;
T0COMM t0comm;

void setup() {                
  	// initialize the digital pin as an output.
  	pinMode(led, OUTPUT);
    t0comm.initiallize();
    delay(3000);
    t0comm.led(1, T0COMM::red, T0COMM::mode_blink);
}

// the loop routine runs over and over again forever:
void loop() {
  	digitalWrite(led, HIGH);
  	delay(1000);
  	digitalWrite(led, LOW);
  	delay(1000);
}
