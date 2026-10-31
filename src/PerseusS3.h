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

    // Toggle logical inversion on every call; takes effect on the next run.
    // Two calls restore the previous inversion setting.
    void reverseLeft() { setLeftInverted(!leftInverted); }
    void reverseRight() { setRightInverted(!rightInverted); }

private:
    bool leftInverted = true;
    bool rightInverted = false;

    // Leave a motor disabled if its PWM initialization failed.
    bool leftReady = false;
    bool rightReady = false;

    // Last DIR levels, including while PWM is zero.
    bool leftDirectionHigh = false;
    bool rightDirectionHigh = false;
};

extern PerseusS3Class PerseusS3;

#endif
