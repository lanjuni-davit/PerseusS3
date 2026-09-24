// Copyright 2026 Davit Lanjuni
// SPDX-License-Identifier: Apache-2.0
// All available methods and commands are documented and explained
// in the README.md file.
#include <Arduino.h>
#include "PerseusS3.h"

void setup() {
    // Initialize both motors
    PerseusS3.beginLeft();
    PerseusS3.beginRight();
}

void loop() {
    // Move forward
    PerseusS3.runLeft(150);
    PerseusS3.runRight(150);
    delay(2000);

    // Brake
    PerseusS3.runLeft(0);
    PerseusS3.runRight(0);
    delay(1000);

    // Move backward
    PerseusS3.runLeft(-150);
    PerseusS3.runRight(-150);
    delay(2000);

    // Brake
    PerseusS3.runLeft(0);
    PerseusS3.runRight(0);
    delay(1000);

    // Turn left
    PerseusS3.runLeft(-100);
    PerseusS3.runRight(100);
    delay(1000);

    // Brake
    PerseusS3.runLeft(0);
    PerseusS3.runRight(0);
    delay(2000);
}