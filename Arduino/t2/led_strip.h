#pragma once
#include <Arduino.h>

class LEDStrip {
public:
    enum Mode { mode_fade, mode_blink };
    enum Color { red, green, blue, white };

    LEDStrip(int pR, int pG, int pB, int pA): pR(pR), pG(pG), pB(pB), pA(pA) {
        pinMode(pR, OUTPUT);
        pinMode(pG, OUTPUT);
        pinMode(pB, OUTPUT);
        pinMode(pA, OUTPUT);
        globalTimer.setOnlyOnce();
        off();
    }

    void rgba(int rval, int gval, int bval, int aval) {
        color[red] = map(rval, 0, 1023, 0, 1023);
        color[green] = map(gval, 0, 1023, 0, 1023);
        color[blue] = map(bval, 0, 1023, 0, 1023);
        color[white] = map(aval, 0, 1023, 0, 1023);
        setValues();
    }

    void on(){ rgba(maxPWM, maxPWM, maxPWM, maxPWM); }
    void off(){ rgba(0, 0, 0, 0); }
    
    void blinkMode(Color color, int time = 50) {
        blinkTimer.reset(time);
        blinkColor = color;
        mode = mode_blink; 
        blinkPos = 1;    
        off(); 
        update();
    }

    void fadeMode(Color color, int time = 50, int inc = 5) {
        fadeInc = inc;
        fadeTimer.reset(time);
        fadeColor = color;
        mode = mode_fade; 
        fadePos = 1;    
        off(); 
        update();
    }

    void setTimer(int time) {
        globalTimer.reset(time);
    }

    void update() {
        if(globalTimer) 
        if (mode == mode_fade) fade();
        if (mode == mode_blink) blink();
    }

protected:
    void setValues() {
        analogWrite(pR, color[red]);
        analogWrite(pG, color[green]);
        analogWrite(pB, color[blue]);
        analogWrite(pA, color[white]);    
    }
      
    void fade() {
        int h = color[fadeColor];
        if (fadeTimer) {
            fadePos = h > maxPWM ? -1 : h  <= 0 ? 1 : fadePos;
            h = h + fadeInc * fadePos;
            color[fadeColor] = map(h, 0, 1023, 0, 1023);
        }  
        setValues();      
    }

    void blink() {
        if (blinkTimer) {
            blinkPos = !blinkPos;
            color[blinkColor] = blinkPos * maxPWM;
        }  
        setValues();      
    }

        
private:
    int mode;
    int color[4];
    int pR, pG, pB, pA;
    int maxPWM = 256;

    int fadeInc = 20;
    int fadePos = 1;
    int fadeColor = 1;
    
    int blinkPos = 1;
    int blinkColor = 1;

    Timer fadeTimer;
    Timer blinkTimer;
    Timer globalTimer;
};
