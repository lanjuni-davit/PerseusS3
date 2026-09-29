// Copyright 2026 Davit Lanjuni
// SPDX-License-Identifier: Apache-2.0
#include "PerseusS3.h"
#include <esp_arduino_version.h>

namespace {


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

// Arduino-ESP32 2.x addresses LEDC by channel; 3.x addresses it by pin.
// Reserve channels 0 and 1 on 2.x. On 3.x the core allocates channels.
constexpr uint8_t LEFT_PWM_CHANNEL = 0;
constexpr uint8_t RIGHT_PWM_CHANNEL = 1;

bool attachMotorPwm(uint8_t pin, uint8_t channel) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    (void)channel;
    if (!ledcAttach(pin, MOTOR_PWM_FREQUENCY, MOTOR_PWM_RESOLUTION)) {
        return false;
    }
    return ledcWrite(pin, 0);
#else
    if (ledcSetup(channel, MOTOR_PWM_FREQUENCY, MOTOR_PWM_RESOLUTION) == 0) {
        return false;
    }
    // Clear any old duty before routing the channel to the motor pin.
    ledcWrite(channel, 0);
    ledcAttachPin(pin, channel);
    return true;
#endif
}

void writeMotorPwm(uint8_t pin, uint8_t channel, uint8_t duty) {
#if ESP_ARDUINO_VERSION_MAJOR >= 3
    (void)channel;
    ledcWrite(pin, duty);
#else
    (void)pin;
    ledcWrite(channel, duty);
#endif
}

// Allow the sensor source, mux output and ADC input to settle together.
// These are conservative starting values, not a measured board guarantee.
constexpr uint32_t MUX_SETTLE_US = 100;
constexpr uint32_t ADC_RECOVERY_US = 10;

}  // namespace


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
    leftReady = false;
    // Keep the driver asleep while configuring its inputs.
    pinMode(SLP_LEFT, OUTPUT);
    digitalWrite(SLP_LEFT, LOW);

    pinMode(EN_LEFT, OUTPUT);
    pinMode(DIR_LEFT, OUTPUT);

    digitalWrite(EN_LEFT, LOW);

    if (!attachMotorPwm(EN_LEFT, LEFT_PWM_CHANNEL)) {
        log_e("PerseusS3: left motor PWM setup failed; driver remains asleep");
        return;
    }

    delayMicroseconds(PWM_SETTLE_US);

    digitalWrite(DIR_LEFT, LOW);
    leftDirectionHigh = false;

    digitalWrite(SLP_LEFT, HIGH);
    delayMicroseconds(DRIVER_WAKE_US);
    leftReady = true;
}

void PerseusS3Class::beginRight() {
    rightReady = false;
    // Keep the driver asleep while configuring its inputs.
    pinMode(SLP_RIGHT, OUTPUT);
    digitalWrite(SLP_RIGHT, LOW);

    pinMode(EN_RIGHT, OUTPUT);
    pinMode(DIR_RIGHT, OUTPUT);

    digitalWrite(EN_RIGHT, LOW);

    if (!attachMotorPwm(EN_RIGHT, RIGHT_PWM_CHANNEL)) {
        log_e("PerseusS3: right motor PWM setup failed; driver remains asleep");
        return;
    }

    delayMicroseconds(PWM_SETTLE_US);

    digitalWrite(DIR_RIGHT, LOW);
    rightDirectionHigh = false;

    digitalWrite(SLP_RIGHT, HIGH);
    delayMicroseconds(DRIVER_WAKE_US);
    rightReady = true;
}


// ============================================================
// Motor control
// ============================================================

// Run the left motor.
//
// pwm range: -255...255. The sign selects direction; magnitude sets duty.
// Zero means electrical braking, not driver disable or coasting.
void PerseusS3Class::runLeft(int16_t pwm) {
    if (!leftReady) {
        return;
    }

    // Prevent values outside the supported PWM range.
    pwm = constrain(pwm, -255, 255);

    if (leftInverted) {
        pwm = -pwm;
    }

    // Keep DIR unchanged when stopping: no unwanted reverse command.
    if (pwm == 0) {
        writeMotorPwm(EN_LEFT, LEFT_PWM_CHANNEL, 0);
        return;
    }

    const bool directionHigh = (pwm > 0);
    if (directionHigh != leftDirectionHigh) {
        writeMotorPwm(EN_LEFT, LEFT_PWM_CHANNEL, 0);
        // ESP32 PWM updates on a timer cycle. Wait two periods (100 us)
        // before changing DIR, even after an immediately preceding stop.
        // This is PWM settling, not enough time to stop a spinning motor.
        delayMicroseconds(PWM_SETTLE_US);
        digitalWrite(DIR_LEFT, directionHigh ? HIGH : LOW);
        leftDirectionHigh = directionHigh;
    }

    // Same-direction commands only update PWM; no extra stop or delay.
    writeMotorPwm(EN_LEFT, LEFT_PWM_CHANNEL, abs(pwm));
}


// Run the right motor.
//
// Uses the same signed PWM command and braking behavior as runLeft().
void PerseusS3Class::runRight(int16_t pwm) {
    if (!rightReady) {
        return;
    }

    // Prevent values outside the supported PWM range.
    pwm = constrain(pwm, -255, 255);

    if (rightInverted) {
        pwm = -pwm;
    }

    if (pwm == 0) {
        writeMotorPwm(EN_RIGHT, RIGHT_PWM_CHANNEL, 0);
        return;
    }

    const bool directionHigh = (pwm > 0);
    if (directionHigh != rightDirectionHigh) {
        writeMotorPwm(EN_RIGHT, RIGHT_PWM_CHANNEL, 0);
        delayMicroseconds(PWM_SETTLE_US);
        digitalWrite(DIR_RIGHT, directionHigh ? HIGH : LOW);
        rightDirectionHigh = directionHigh;
    }

    writeMotorPwm(EN_RIGHT, RIGHT_PWM_CHANNEL, abs(pwm));
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
    // This board uses channels 0–11.
    if (index > 11) {
        return -1;
    }

static constexpr uint8_t channelSelect[12][4] = {
    // S0 S1 S2 S3       Physical index → mux channel
    { 0, 0, 0, 0 },  //  0 → 0
    { 1, 0, 0, 0 },  //  1 → 1
    { 0, 1, 0, 0 },  //  2 → 2
    { 1, 1, 0, 0 },  //  3 → 3
    { 0, 0, 1, 0 },  //  4 → 4
    { 1, 1, 1, 0 },  //  5 → 7
    { 0, 1, 1, 0 },  //  6 → 6
    { 1, 0, 1, 0 },  //  7 → 5
    { 0, 0, 0, 1 },  //  8 → 8
    { 1, 0, 0, 1 },  //  9 → 9
    { 0, 1, 0, 1 },  // 10 → 10
    { 1, 1, 0, 1 }   // 11 → 11
};

    // selectorPins[0..3] must connect to S0, S1, S2, S3.
    // The multiplexer enable input E̅ must already be LOW.
    for (uint8_t pin = 0; pin < 4; ++pin) {
        digitalWrite(
            selectorPins[pin],
            channelSelect[index][pin] ? HIGH : LOW
        );
    }

    delayMicroseconds(MUX_SETTLE_US);

    // Discard the first conversion after selecting the channel.
    (void)analogRead(sensorInputPin);
    delayMicroseconds(ADC_RECOVERY_US);

    return analogRead(sensorInputPin);
}


// ============================================================
// Global library object
// ============================================================
PerseusS3Class PerseusS3;
