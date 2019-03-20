#pragma once
#include <Arduino.h>

#define MAX_PWM 1023

namespace LEDStrip {

struct RGBWColor {
   uint8_t r, g, b, a;
   
   RGBWColor(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 0) : r(r), g(g), b(b), a(a) {}
   
   void set(uint8_t rval, uint8_t gval, uint8_t bval, uint8_t aval = 0) {
       r = rval;
       g = gval;
       b = bval;
       a = aval;
   }
};

// common colors
RGBWColor BLACK(0, 0, 0);
RGBWColor WHITE(255, 255, 255);
RGBWColor WHITEONLY(0, 0, 0, 255);
RGBWColor WHITEFULL(255, 255, 255, 255);
RGBWColor RED(255, 0, 0);
RGBWColor LIME(0, 255, 0);
RGBWColor BLUE(0, 0, 255);
RGBWColor YELLOW(255, 255, 0);
RGBWColor CYAN(0, 255, 255);
RGBWColor MAGENTA(255, 0, 255);
RGBWColor MAROON(128, 0, 0);
RGBWColor OLIVE(128, 128, 0);
RGBWColor GREEN(0, 128, 0);
RGBWColor PURPLE(128, 0, 128);
RGBWColor TEAL(0, 128, 128);
RGBWColor NAVY(0, 0, 128);
RGBWColor ORANGE(255, 69, 0);


// transitional table
const uint8_t transitionColorsSize = 6;
const uint8_t transitionSetSize = 4;
const RGBWColor transitionTable[transitionSetSize][transitionColorsSize] =
{
    {RED, LIME, BLUE, YELLOW, CYAN, MAGENTA},
    {MARRON, OLIVE, GREEN, PURPLE, TEAL, NAVY},
    {RED, ORANGE, LIME, GREEN, BLUE, TEAL},
    {RED, BLUE, RED, BLUE, RED, BLUE} // police mode
};


class LEDStrip {
public:
    // Default constructor
    LEDStrip() : color(BLACK) {
        offTimer.setResetWhenDone(false);
    }

    // Constructor with pins
    LEDStrip(uint8_t pR, uint8_t pG, uint8_t pB, uint8_t pA) : pR(pR), pG(pG), pB(pB), pA(pA), color(BLACK) {
        pinMode(pR, OUTPUT);
        pinMode(pG, OUTPUT);
        pinMode(pB, OUTPUT);
        pinMode(pA, OUTPUT);
        offTimer.setResetWhenDone(false);
    }

    // Set LEDStrip PWM pins
    void setPins(uint8_t pinR, uint8_t pinG, uint8_t pinB, uint8_t pinA) {
        pR = pinR;
        pG = pinG;
        pB = pinB;
        pA = pinA;
        pinMode(pR, OUTPUT);
        pinMode(pG, OUTPUT);
        pinMode(pB, OUTPUT);
        pinMode(pA, OUTPUT);     
    }

    // disable led strip. Doesn't clear modes
    void off() { 
        ledEnable = false; 
        update();
    }

    // enable led strip. Doesn't clear modes
    void on() { 
        ledEnable = true; 
        update();
    }

    // turn off led strip after certain time
    void off(uint16_t time) {
        offTimer.reset(time);
        update();
    }
    
    // set color with rgba values. Clears all modes
    void rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0) { 
        clearCurrentMode(); 
        color.set(r, g, b, a);
        update();
    }

    // set color to red. Clears all modes
    void red() { 
        clearCurrentMode();
        color = RED;
        update();
    }

    // set color to green. Clears all modes
    void green() {
        clearCurrentMode();
        color = GREEN;
        update();
    }

    // set color to blue. Clears all modes
    void blue() {
        clearCurrentMode();
        color = BLUE;
        update();
    }

    // set color to white. Clears all modes
    void white() {
        clearCurrentMode();
        color = WHITE;
        update();
    }

    // set color to white only led. Clears all modes
    void whiteOnly() {
        clearCurrentMode();
        color = WHITEONLY;
        update();
    }

    // set color to white all leds. Clears all modes
    void whiteFull() {
        clearCurrentMode();
        color = WHITEFULL;
        update();
    }

    // dim the LED brightness
    void dim(uint8_t v) {
        dimValue = clip(v, 0, 255);
        update();
    }
   
    // blink current color on an off independently of the mode
    // does not clear current mode
    void blinkMode(bool enable, uint16_t time = 500) {
        blinkEnable = enable;
        blinkLEDOn = true;
        blinkTimer.reset(time);
        update();
    }

    // jump from one color to the next in a list of colors
    // clears all modes
    void jumpMode(uint8_t set, uint16_t time = 500) {
        clearCurrentMode(); 
        jumpEnable = true;
        jumpSet = clip(set, 0, transitionSetSize - 1);
        jumpIndex = 0;
        jumpTimer.reset(time);
        update();
    }

    // smooth transition from one color to the next in a list of colors
    // clears all modes
    void smoothMode(uint8_t set, uint16_t time = 500) {
        clearCurrentMode(); 
        smoothEnable = true;
        smoothSet = clip(set, 0, transitionSetSize - 1);
        smoothIndex = 0;
        smoothTimer.reset(time);
        update();
    }

    // fade on an off from one color to the next in a list of colors
    // clears all modes
    void fadeMode(uint8_t set = 0, uint16_t time = 500) {
        clearCurrentMode(); 
        fadeEnable = true;
        fadeSet = clip(set, 0, transitionSetSize - 1);
        fadeIndex = 0;
        fadeTimer.reset(time);
        update(); 
    }

    // fade on an off current color
    // clears all modes
    void fadeCurrentMode(uint16_t time = 500) {
        clearCurrentMode(); 
        fadeCurrentEnable = true;
        fadeCurrentColor = color;
        fadeCurrentIndex = 0;
        fadeCurrentTimer.reset(time);
        update();
    }

    // Update the color based on the current mode
    void update() {
        if (ledEnable) {
            if (offTimer) {
                off();
                return;
            }

            if (blinkEnable && blinkTimer) {
                blinkLEDOn = !blinkLEDOn
                if (!blinkLEDOn) return;
            }

            if (jumpEnable) {
                if (jumpTimer) jumpIndex = (jumpIndex + 1) % transitionColorsSize;
                color = transitionTable[jumpSet][jumpIndex];
            }

            if (smoothEnable) {                
                if (smoothTimer) {
                    smoothIndex = (smoothIndex + 1) % transitionColorsSize;
                    color = transitionTable[smoothSet][smoothIndex];
                }
                uint8_t smoothIndexNext = (smoothIndex + 1) % transitionColorsSize;
                RGBWColor nextColor = transitionTable[smoothSet][smoothIndexNext];
                float et = smoothTimer.elapsedPercentage();
                color = interpolateRGB(color, nextColor, et);
            }

            if (fadeEnable) {                
                if (fadeTimer) {
                    fadeIndex = (fadeIndex + 1) % transitionColorsSize;
                    color = transitionTable[fadeSet][fadeIndex];
                }
                float et = fadeTimer.elapsedPercentage();
                if (et < 0.5)
                    color = interpolateRGB(BLACK, color, 2 * et);
                else
                    color = interpolateRGB(color, BLACK, 1 - 2 * et);
            }

            if (fadeCurrentEnable) {                
                if (fadeCurrentTimer)
                    color = fadeCurrentColor;
                float et = fadeCurrentTimer.elapsedPercentage();
                if (et < 0.5)
                    color = interpolateRGB(BLACK, color, 2 * et);
                else
                    color = interpolateRGB(color, BLACK, 1 - 2 * et);
            }

            writeValues();
        }
    }

protected:
    void clearCurrentMode() {
        smoothEnable = false;
        jumpEnable = false;
        fadeEnable = false;
        fadeCurrentEnable = false;
    }

    uint8_t colorToPWM(uint8_t color) {
        return clip(color / 255.0 * dimValue / 255.0 * MAX_PWM, 0, MAX_PWM);
    }

    RGBWColor interpolateRGB(const RGBWColor &a, const RGBWColor &b, float t) {
        RGBWColor c;
        c.r = a.r * (1 - t) + b.r * t;
        c.g = a.g * (1 - t) + b.g * t;
        c.b = a.b * (1 - t) + b.b * t;
        c.a = a.a * (1 - t) + b.a * t;
        return c;
    }

    void writeValues() {
        analogWrite(pR, colorToPWM(color.r));
        analogWrite(pG, colorToPWM(color.g));
        analogWrite(pB, colorToPWM(color.b));
        analogWrite(pA, colorToPWM(color.a));
    }
private:
    RGBWColor color;
    uint8_t pR, pG, pB, pA;
    uint8_t dimValue = 50;
    bool ledEnable = true;

    Timer offTimer;

    bool blinkOnceEnable = false;
    bool blinkOnceLEDOn = false;
    Timer blinkOnceTimer;

    bool jumpEnable = false;
    uint8_t jumpSet = 0;
    uint8_t jumpIndex = 0;
    Timer jumpTimer;

    bool smoothEnable = false;
    uint8_t smoothSet = 0;
    uint8_t smoothIndex = 0;
    Timer smoothTimer;

    bool fadeEnable = false;
    uint8_t fadeSet = 0;
    uint8_t fadeIndex = 0;
    Timer fadeTimer;

    bool fadeCurrentEnable = true;
    RGBWColor fadeCurrentColor;
    uint8_t fadeCurrentIndex = 0;
    Timer fadeCurrentTimer
};

} // namespace LEDStrip