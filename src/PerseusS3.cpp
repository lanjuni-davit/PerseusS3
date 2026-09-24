// Copyright 2026 Davit Lanjuni
// SPDX-License-Identifier: Apache-2.0
#include "PerseusS3.h"


// ============================================================
// Motor pin definitions
// ============================================================

// Left motor control pins
constexpr uint8_t EN_LEFT  = 11;  // PWM / speed control
constexpr uint8_t DIR_LEFT = 10;  // Direction control
constexpr uint8_t SLP_LEFT = 12;  // Sleep / enable control

// Right motor control pins
constexpr uint8_t EN_RIGHT  = 15;  // PWM / speed control
constexpr uint8_t DIR_RIGHT = 14;  // Direction control
constexpr uint8_t SLP_RIGHT = 13;  // Sleep / enable control

// Fixed PWM settings make the zero-duty settling time predictable.
// Keep these motor pins/timers under library control after begin*().
constexpr uint32_t MOTOR_PWM_FREQUENCY = 20000;
constexpr uint8_t MOTOR_PWM_RESOLUTION = 8;
constexpr uint32_t PWM_SETTLE_US = (2000000UL + MOTOR_PWM_FREQUENCY - 1) / MOTOR_PWM_FREQUENCY;
constexpr uint32_t DRIVER_WAKE_US = 500;  // MP6612D maximum start-up delay.


// ============================================================
// Constructor
// ============================================================

PerseusS3Class::PerseusS3Class() {
}


// ============================================================
// Motor direction configuration
// ============================================================

// Set or clear logical inversion without changing the current motor command.
void PerseusS3Class::setLeftInverted(bool inverted) {
    leftInverted = inverted;
}

void PerseusS3Class::setRightInverted(bool inverted) {
    rightInverted = inverted;
}


// ============================================================
// Motor initialization
// ============================================================

void PerseusS3Class::beginLeft() {
    // Keep the driver asleep while configuring its inputs.
    pinMode(SLP_LEFT, OUTPUT);
    digitalWrite(SLP_LEFT, LOW);

    pinMode(EN_LEFT, OUTPUT);
    pinMode(DIR_LEFT, OUTPUT);

    digitalWrite(EN_LEFT, LOW);

    analogWriteResolution(EN_LEFT, MOTOR_PWM_RESOLUTION);
    analogWriteFrequency(EN_LEFT, MOTOR_PWM_FREQUENCY);
    analogWrite(EN_LEFT, 0);

    delayMicroseconds(PWM_SETTLE_US);

    digitalWrite(DIR_LEFT, LOW);
    leftDirectionHigh = false;

    digitalWrite(SLP_LEFT, HIGH);
    delayMicroseconds(DRIVER_WAKE_US);
}

void PerseusS3Class::beginRight() {
    // Keep the driver asleep while configuring its inputs.
    pinMode(SLP_RIGHT, OUTPUT);
    digitalWrite(SLP_RIGHT, LOW);

    pinMode(EN_RIGHT, OUTPUT);
    pinMode(DIR_RIGHT, OUTPUT);

    digitalWrite(EN_RIGHT, LOW);

    analogWriteResolution(EN_RIGHT, MOTOR_PWM_RESOLUTION);
    analogWriteFrequency(EN_RIGHT, MOTOR_PWM_FREQUENCY);
    analogWrite(EN_RIGHT, 0);

    delayMicroseconds(PWM_SETTLE_US);

    digitalWrite(DIR_RIGHT, LOW);
    rightDirectionHigh = false;

    digitalWrite(SLP_RIGHT, HIGH);
    delayMicroseconds(DRIVER_WAKE_US);
}


// ============================================================
// Motor control
// ============================================================

// Run the left motor.
//
// pwm range: -255...255. The sign selects direction; magnitude sets duty.
// Zero means electrical braking, not driver disable or coasting.
void PerseusS3Class::runLeft(int16_t pwm) {

    // Prevent values outside the supported PWM range.
    pwm = constrain(pwm, -255, 255);

    if (leftInverted) {
        pwm = -pwm;
    }

    // Keep DIR unchanged when stopping: no unwanted reverse command.
    if (pwm == 0) {
        analogWrite(EN_LEFT, 0);
        return;
    }

    const bool directionHigh = (pwm > 0);
    if (directionHigh != leftDirectionHigh) {
        analogWrite(EN_LEFT, 0);
        // ESP32 PWM updates on a timer cycle. Wait two periods (100 us)
        // before changing DIR, even after an immediately preceding stop.
        // This is PWM settling, not enough time to stop a spinning motor.
        delayMicroseconds(PWM_SETTLE_US);
        digitalWrite(DIR_LEFT, directionHigh ? HIGH : LOW);
        leftDirectionHigh = directionHigh;
    }

    // Same-direction commands only update PWM; no extra stop or delay.
    analogWrite(EN_LEFT, abs(pwm));
}


// Run the right motor.
//
// Uses the same signed PWM command and braking behavior as runLeft().
void PerseusS3Class::runRight(int16_t pwm) {

    // Prevent values outside the supported PWM range.
    pwm = constrain(pwm, -255, 255);

    if (rightInverted) {
        pwm = -pwm;
    }

    if (pwm == 0) {
        analogWrite(EN_RIGHT, 0);
        return;
    }

    const bool directionHigh = (pwm > 0);
    if (directionHigh != rightDirectionHigh) {
        analogWrite(EN_RIGHT, 0);
        delayMicroseconds(PWM_SETTLE_US);
        digitalWrite(DIR_RIGHT, directionHigh ? HIGH : LOW);
        rightDirectionHigh = directionHigh;
    }

    analogWrite(EN_RIGHT, abs(pwm));
}


// ============================================================
// Line sensor multiplexer initialization
// ============================================================

// Initialize the line sensor multiplexer.
//
// S0-S3:
//   Multiplexer channel-selection pins.
//
// inputPin:
//   Analog output of the multiplexer connected to the ESP32 ADC.
void PerseusS3Class::beginLineSensors(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t inputPin) {

    // Save the selected pins so the library can use them later.
    selectorPins[0] = s0;
    selectorPins[1] = s1;
    selectorPins[2] = s2;
    selectorPins[3] = s3;
    sensorInputPin = inputPin;

    // Start with multiplexer channel 0 selected.
    for (uint8_t i = 0; i < 4; ++i) {
        pinMode(selectorPins[i], OUTPUT);
        digitalWrite(selectorPins[i], LOW);
    }

    // Multiplexer signal output is read by the ADC.
    pinMode(sensorInputPin, INPUT);
}


// ============================================================
// ADC resolution
// ============================================================

// Set ADC resolution used for reading the line sensors.
//
// bits is limited to 1-15 bits so that the maximum ADC value
// still fits inside the positive range of int16_t.
void PerseusS3Class::setAdcResolution(uint8_t bits) {

    // Keep resolution inside the supported library range.
    bits = constrain(bits, 1, 15);

    // This changes the ADC return width, not optical sensitivity.
    analogReadResolution(bits);
}


// ============================================================
// Line sensor reading
// ============================================================

// Read one of the 12 connected line sensors.
//
// Valid sensor indexes:
//   0 through 11
//
// Returns:
//   ADC reading for the selected sensor.
//
//   -1 if an invalid sensor index is requested.
int16_t PerseusS3Class::readLineSensor(uint8_t index) {

    // Only channels 0-11 are physically connected.
    if (index > 11) {
        return -1;
    }

    // Convert the channel number into the four multiplexer
    // selection bits S0-S3.
    //
    // Example:
    // index 5 -> binary 0101
    // S0 = 1
    // S1 = 0
    // S2 = 1
    // S3 = 0
    digitalWrite(selectorPins[0], index % 2);
    digitalWrite(selectorPins[1], (index / 2) % 2);
    digitalWrite(selectorPins[2], (index / 4) % 2);
    digitalWrite(selectorPins[3], (index / 8) % 2);

    // Give the multiplexer output and ADC input time to settle
    // after switching channels.
    delayMicroseconds(10);

    // Read and return the selected sensor value.
    return analogRead(sensorInputPin);
}


// ============================================================
// Global library object
// ============================================================
PerseusS3Class PerseusS3;
