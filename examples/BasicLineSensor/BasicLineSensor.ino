// Copyright 2026 Davit Lanjuni
// SPDX-License-Identifier: Apache-2.0
// All available methods and commands are documented and explained
// in the README.md file.

#include <Arduino.h>
#include "PerseusS3.h"

void setup() {
    Serial.begin(115200);

    // Initialize line sensor multiplexer
    // S0, S1, S2, S3, ADC input
    PerseusS3.beginLineSensors(37, 36, 35, 34, 4);

    // Set ADC resolution
    PerseusS3.setAdcResolution(12);
}

void loop() {
    // Read all 12 line sensors
    for (uint8_t i = 0; i < 12; i++) {
        int value = PerseusS3.readLineSensor(i);

        Serial.print("S");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(value);
        Serial.print("  ");
    }

    Serial.println();

    delay(100);
}