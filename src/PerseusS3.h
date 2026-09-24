// Copyright 2026 Davit Lanjuni
// SPDX-License-Identifier: Apache-2.0
#ifndef PERSEUSS3_H
#define PERSEUSS3_H

#include <Arduino.h>
#include <cstdint>

class PerseusS3Class {
public:
    PerseusS3Class();

    // Configure direction inversion; takes effect on the next run command.
    void setLeftInverted(bool inverted);
    void setRightInverted(bool inverted);

    // Initialize the driver with PWM = 0 and DIR = LOW before enabling it.
    void beginLeft();
    void beginRight();

    // Signed PWM command, clamped to -255...255; not measured motor speed.
    // Call beginLeft()/beginRight() first. Zero brakes the MP6612D motor.
    void runLeft(int16_t pwm);
    void runRight(int16_t pwm);

    // Initialize the line sensor multiplexer with default or custom pins.
    void beginLineSensors(uint8_t s0 = 36, uint8_t s1 = 35, uint8_t s2 = 34, uint8_t s3 = 33, uint8_t inputPin = 7);

    // Set ADC reading resolution in bits, clamped to 1-15.
    void setAdcResolution(uint8_t bits);

    // Read raw ADC data from sensor 0-11 after beginLineSensors().
    // Returns -1 for an invalid index.
    int16_t readLineSensor(uint8_t index);

    // Compatibility with existing sketches. reverse*() enables inversion;
    // repeated calls do not toggle it. Use set*Inverted(false) to clear it.
    void reverseLeft() { setLeftInverted(true); }
    void reverseRight() { setRightInverted(true); }
    void linebegin(uint8_t s0 = 36, uint8_t s1 = 35, uint8_t s2 = 34, uint8_t s3 = 33, uint8_t inputPin = 7) {
        beginLineSensors(s0, s1, s2, s3, inputPin);
    }
    int16_t linegetState(uint8_t index) { return readLineSensor(index); }

private:
    bool leftInverted = true;
    bool rightInverted = false;

    // Last DIR levels, including while PWM is zero.
    bool leftDirectionHigh = false;
    bool rightDirectionHigh = false;

    // Multiplexer selector pins, ordered S0, S1, S2, S3.
    uint8_t selectorPins[4] = {36, 35, 34, 33};

    // ADC pin connected to the multiplexer output.
    uint8_t sensorInputPin = 7;
};

extern PerseusS3Class PerseusS3;

#endif
