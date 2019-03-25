#pragma once
#include <Arduino.h> 

// Front Right Motor Pins
#define INARFR 23    // EnaA
#define INBRFR 20    // EnaB
#define PWMRFR 22    // PWM
#define CSRFR  A7    // Current Signal Analog
#define AERFR  2     // Encoder A
#define BERFR  3     // Encoder B
#define SIGFR  24    // Endstop signal Pull-Up

// Front Left Motor Pins
#define INARFL 17    // EnaA
#define INBRFL 14    // EnaB
#define PWMRFL 16    // PWM
#define CSRFL  A1    // Current Signal Analog
#define AERFL  4     // Encoder A
#define BERFL  5     // Encoder B
#define SIGFL  25    // Endstop signal Pull-Up

// Back Right Motor Pins
#define INARBR 13    // EnaA
#define INBRBR 37    // EnaB
#define PWMRBR 38    // PWM
#define CSRBR  A20   // Current Signal Analog
#define AERBR  7     // Encoder A
#define BERBR  6     // Encoder B
#define SIGBR  26    // Endstop signal Pull-Up

// Back Left Motor Pins
#define INARBL 36    // EnaA
#define INBRBL 33    // EnaB
#define PWMRBL 35    // PWM
#define CSRBL  A15   // Current Signal Analog
#define AERBL  9     // Encoder A
#define BERBL  8     // Encoder B
#define SIGBL  27    // Endstop signal Pull-Up

// I2C Pins
#define I2C_SLC SLC0
#define I2C_SDA SDA0

// Sonar Serial
#define SonarSerial Serial4
