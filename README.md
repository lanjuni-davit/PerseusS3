![PerseusS3](https://raw.githubusercontent.com/lanjuni-davit/PerseusS3/master/Images/PerseusForge.png)

# PerseusS3

Arduino library for the **PerseusS3 robotics development board**, based on the
ESP32-S3. It provides signed PWM motor control, direction inversion, and readings
from a 12-channel analog line-sensor interface.

## Hardware overview

| Specification | PerseusS3 |
|---|---|
| MCU module | ESP32-S3-MINI-1U-N8 |
| CPU | Dual-core Xtensa LX7, up to 240 MHz |
| Flash | 8 MB |
| Wireless | 2.4 GHz Wi-Fi and Bluetooth Low Energy |
| GPIO logic level | 3.3 V |
| Specified board input voltage | 4–35 V |
| On-board regulator | 3.3 V, up to 2 A |
| Motor channels | Two brushed DC motors |
| Motor drivers | MP6612D |
| Line-sensor channels used by the library | 12, indexed 0–11 |
| USB connector | USB Type-C |
| Approximate dimensions | 80 × 27 × 9.9 mm |
| Framework | Arduino for ESP32 |

The MP6612D is rated for up to **5 A continuous current per driver under suitable
electrical and thermal conditions**. Actual board capability depends on cooling,
PCB design, ambient temperature and motor load. Its **14.5 A minimum over-current
threshold with OC_ADJ connected to GND** is a protection threshold, not a usable
continuous-current rating.

Keep ESP32-S3 GPIO signals within **3.3 V** and share ground between the board,
line sensors and multiplexer. The board's specified 4–35 V supply range is
different from the voltage limits of its individual components.

## Installation

### Arduino IDE

1. Install **esp32 by Espressif Systems** through Boards Manager.
2. Download the library ZIP and use **Sketch > Include Library > Add .ZIP Library**.
3. Select the appropriate ESP32-S3 board profile and its flash/USB settings.
   The module documented above has 8 MB flash.
4. Include the library in your sketch:

```cpp
#include <Arduino.h>
#include <PerseusS3.h>
```

When replacing an older installation, avoid keeping multiple copies of the
library in the Arduino libraries folder.

### PlatformIO

For a local installation, put the library folder at `lib/PerseusS3/` inside your
PlatformIO project. Keep its `src/` folder and `library.properties` intact.

This example configuration uses a generic ESP32-S3 profile with 8 MB flash:

```ini
[env:perseuss3]
platform = espressif32@7.0.1
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200
```

This is a generic build profile, not a custom PerseusS3 board definition. Verify
the board and USB settings against your hardware. Put your sketch in `src/main.cpp`
and include both `Arduino.h` and `PerseusS3.h`.

To fetch a copy from GitHub instead, add this to the environment:

```ini
lib_deps =
    https://github.com/lanjuni-davit/PerseusS3.git
```

Use a repository revision containing the implementation documented here. Avoid
selecting an older remote copy while testing a corrected local copy. Pin a commit
or tag when you need a reproducible dependency.

## ESP32 core compatibility

PWM configuration selects the Arduino-ESP32 API at compile time:

| ESP32 core | PWM setup | PWM updates |
|---|---|---|
| 2.x | `ledcSetup()` and `ledcAttachPin()` | `ledcWrite(channel, duty)` |
| 3.x | `ledcAttach()` | `ledcWrite(pin, duty)` |

This selection depends on the installed **ESP32 core version**, regardless of
whether you use Arduino IDE or PlatformIO.

On core 2.x, motor control reserves **LEDC channels 0 and 1 and their shared
timer**. On core 3.x, the core allocates channels automatically. Other code must
not reconfigure the motor pins or their PWM timers after initialization.

### Verification status

| Check | Result |
|---|---|
| Library and both basic examples with real ESP32-S3 GCC and core 2.0.17 | Compiled into object files |
| Direction-inversion example with core 2.0.17 | Compiled into an object file |
| 2.x and 3.x API branches using test interfaces | Compiler checks passed |
| Full Arduino-ESP32 3.x firmware build | Not yet verified |
| Complete local PlatformIO build | Blocked by a local Windows compiler subprocess error |
| Physical motor and sensor measurements | Still required |

The included workflow defines example builds for cores 2.0.17, 3.0.7 and 3.3.12,
plus regression tests. Those workflow runs and runtime tests are not claimed as
completed by the checks above.

## Motor control

### Motor GPIO assignments

| Signal | Left motor | Right motor |
|---|---:|---:|
| EN / PWM | GPIO 11 | GPIO 15 |
| DIR | GPIO 10 | GPIO 14 |
| SLP | GPIO 12 | GPIO 13 |

Call `beginLeft()` and `beginRight()` before their respective run methods. The
library configures **20 kHz PWM with 8-bit resolution** while holding each driver
asleep, starts with zero duty, then wakes the driver.

If PWM setup fails, the affected driver stays asleep, an error is logged, and
run commands for that motor are ignored until initialization succeeds.

### Signed PWM commands

```cpp
PerseusS3.runLeft(150);
PerseusS3.runRight(150);

PerseusS3.runLeft(-150);
PerseusS3.runRight(-150);

PerseusS3.runLeft(0);
PerseusS3.runRight(0);
```

- The accepted command range is **−255 to +255**; larger magnitudes are clamped.
- The sign selects direction, after applying the motor's inversion setting.
- The magnitude sets PWM duty. It does not specify a measured speed in RPM.
- **Zero applies electrical braking**, keeps the last DIR level and leaves the
  driver enabled.
- Physical forward/backward motion depends on motor wiring and mounting.

For a direction change, PWM is set to zero and the library waits **100 µs** before
changing DIR and applying the new duty. This interval allows PWM to settle; it
does not guarantee that a moving motor has mechanically stopped. Updating duty
without changing direction does not add this delay.

### Basic motor example

```cpp
#include <Arduino.h>
#include <PerseusS3.h>

void setup() {
    PerseusS3.beginLeft();
    PerseusS3.beginRight();
}

void loop() {
    // One logical direction.
    PerseusS3.runLeft(150);
    PerseusS3.runRight(150);
    delay(2000);

    PerseusS3.runLeft(0);
    PerseusS3.runRight(0);
    delay(1000);

    // Opposite logical direction.
    PerseusS3.runLeft(-150);
    PerseusS3.runRight(-150);
    delay(2000);

    PerseusS3.runLeft(0);
    PerseusS3.runRight(0);
    delay(1000);
}
```

## Direction inversion and reverse toggles

The default settings are **left inverted = true** and **right inverted = false**.

Use the setters to choose a specific state:

```cpp
PerseusS3.setLeftInverted(true);    // Enable left inversion.
PerseusS3.setRightInverted(true);   // Enable right inversion.

PerseusS3.setLeftInverted(false);   // Disable left inversion.
PerseusS3.setRightInverted(false);  // Disable right inversion.
```

Repeatedly setting `true` keeps inversion enabled. Repeatedly setting `false`
keeps it disabled.

Use the reverse methods to **toggle the current state on every call**:

```cpp
PerseusS3.reverseLeft();
PerseusS3.reverseRight();
```

| Before a reverse call | After the call |
|---|---|
| Inversion disabled (`false`) | Inversion enabled (`true`) |
| Inversion enabled (`true`) | Inversion disabled (`false`) |

Both setters and toggles take effect on the **next run command**. They do not
immediately change an already-running motor. Calling the same reverse method
twice restores its previous inversion setting. If both calls occur before the
next run command, the motor sees no direction change from those two toggles.

### Inversion example

This demonstration explicitly starts with inversion disabled on both motors.
It uses the same positive PWM command throughout:

```cpp
#include <Arduino.h>
#include <PerseusS3.h>

void runThenBrake() {
    PerseusS3.runLeft(100);
    PerseusS3.runRight(100);
    delay(1000);

    PerseusS3.runLeft(0);
    PerseusS3.runRight(0);
    delay(500);
}

void setup() {
    PerseusS3.beginLeft();
    PerseusS3.beginRight();
    PerseusS3.setLeftInverted(false);
    PerseusS3.setRightInverted(false);
}

void loop() {
    runThenBrake();  // Original directions.

    PerseusS3.reverseLeft();   // false -> true
    PerseusS3.reverseRight();  // false -> true
    runThenBrake();            // Opposite directions.

    PerseusS3.reverseLeft();   // true -> false
    PerseusS3.reverseRight();  // true -> false
    runThenBrake();            // Original directions again.
}
```

## Line-sensor wiring

The multiplexer connects one sensor at a time to the ESP32 ADC. The library
reads **channels 0–11**. An invalid index returns `-1` without changing the
selector pins or reading the ADC.

### Default pins used by the library

| Multiplexer signal | ESP32 GPIO / connection |
|---|---|
| S0 | GPIO 36 |
| S1 | GPIO 35 |
| S2 | GPIO 34 |
| S3 | GPIO 33 |
| Common analog output to ADC | GPIO 7 |
| E / EN, active LOW | Connect to GND for continuous operation |
| GND | Common ground with ESP32 and sensors |

**Do not leave the multiplexer E/EN pin floating.** LOW enables the selected
channel; HIGH disconnects all channels. An unconnected enable input can disable
the signal path unpredictably and leave the ADC input floating. This can produce
large jumps even when the sensors are stationary.

The library has no multiplexer-enable GPIO argument. For continuous operation,
connect E/EN to GND with power off. If your design controls enable through an
ESP32 GPIO, configure and drive that GPIO LOW in your sketch before reading.
The multiplexer E/EN pin is separate from the motor-driver EN/PWM pins.

### Initialization

Use the defaults when they match your wiring:

```cpp
PerseusS3.beginLineSensors();
```

For different wiring, the argument order is **S0, S1, S2, S3, ADC input**:

```cpp
// Example custom wiring only; use your actual GPIO numbers.
PerseusS3.beginLineSensors(37, 36, 35, 34, 4);
```

The library cannot detect which GPIO is physically connected to each selector.
Verify these connections against the board schematic or mux pins.

### Channel selection order

The following columns are in **S0, S1, S2, S3** order. S0 has weight 1, S1 weight
2, S2 weight 4 and S3 weight 8. Every row requires E/EN to be LOW.

| Channel | S0 | S1 | S2 | S3 |
|---:|---:|---:|---:|---:|
| 0 | 0 | 0 | 0 | 0 |
| 1 | 1 | 0 | 0 | 0 |
| 2 | 0 | 1 | 0 | 0 |
| 3 | 1 | 1 | 0 | 0 |
| 4 | 0 | 0 | 1 | 0 |
| 5 | 1 | 0 | 1 | 0 |
| 6 | 0 | 1 | 1 | 0 |
| 7 | 1 | 1 | 1 | 0 |
| 8 | 0 | 0 | 0 | 1 |
| 9 | 1 | 0 | 0 | 1 |
| 10 | 0 | 1 | 0 | 1 |
| 11 | 1 | 1 | 0 | 1 |

For channel 5, the pin values read left to right are **1, 0, 1, 0**. The library
uses an explicit lookup table with this order. Pins are written sequentially;
the ADC is read only after all four writes and the settling delay.

Channels 12–15 exist on the 16-channel multiplexer but are outside this library's
12-sensor interface. An incorrect selector-pin order can accidentally select one
of those inputs. For example, swapping physical S0 and S3 maps requested channel
5 onto multiplexer channel 12.

## Reading line sensors

```cpp
int16_t value = PerseusS3.readLineSensor(5);
```

Each valid call follows this sequence:

1. Write the channel's S0–S3 selector values.
2. Wait `MUX_SETTLE_US` (**100 µs**).
3. Take and discard one ADC conversion.
4. Wait `ADC_RECOVERY_US` (**10 µs**).
5. Take and return a fresh ADC conversion.

Both constants are in microseconds. The discarded sample is not averaged into
the returned value. This sequence also runs when reading the same channel again.
The delay values are starting points that require verification with the actual
sensor output impedance, multiplexer and wiring. They cannot correct a floating
enable pin or an incorrect connection.

Read the multiplexer from one task at a time. Another task changing the selector
pins during a read can change which sensor is measured.

### Sensor example

Connect E/EN to GND and confirm the default pins before running this example.
Open Serial Monitor at **115200 baud**.

```cpp
#include <Arduino.h>
#include <PerseusS3.h>

void setup() {
    Serial.begin(115200);
    PerseusS3.beginLineSensors();
    PerseusS3.setAdcResolution(12);
}

void loop() {
    for (uint8_t channel = 0; channel < 12; ++channel) {
        int16_t value = PerseusS3.readLineSensor(channel);
        Serial.print("CH");
        Serial.print(channel);
        Serial.print(": ");
        Serial.print(value);
        Serial.print("  ");
    }
    Serial.println();
    delay(100);
}
```

### Troubleshooting unstable readings

| Symptom | Check first |
|---|---|
| Large, random jumps on a stationary surface | E/EN held LOW, common ground and ADC/common connection |
| Some channels fluctuate while others are stable | Selector GPIO order, sensor connections and accidentally selected unused inputs |
| One channel is stable but scans are unstable | Verify wiring, then compare longer settling times and successive conversions |
| The wrong channel consistently responds to a physical sensor | Physical sensor numbering and S0–S3 wiring |
| Instability appears when motors run | Sensor supply and ground noise from the motor system |

First test with motors off and the sensor array stationary on a uniform surface.
Compare repeated reads of one channel with a full scan. Black/white polarity and
thresholds depend on the sensor circuit; determine them from stable measurements.
Confirm the multiplexer supply and input-logic requirements for its exact part
number, and keep the signal reaching the ESP32 ADC within its supported range.

## ADC resolution

```cpp
PerseusS3.setAdcResolution(12);
```

The argument is clamped to **1–15 bits** so that valid results fit in the positive
range of `int16_t`, leaving `-1` available for an invalid sensor index.

| Requested return width | Nominal numeric range |
|---:|---:|
| 8 bits | 0–255 |
| 10 bits | 0–1023 |
| 12 bits | 0–4095 |
| 15 bits | 0–32767 |

The ESP32-S3 has a native 12-bit ADC. Choosing a wider return value scales the
number; it does not create additional measurement accuracy or change the sensor's
optical sensitivity. This method calls the core's `analogReadResolution()` and
affects other `analogRead()` calls too. Keep that shared setting within the
library's supported range while using `readLineSensor()`.

## API reference

| Method | Behavior |
|---|---|
| `beginLeft()` | Initialize left motor PWM and wake its driver on success |
| `beginRight()` | Initialize right motor PWM and wake its driver on success |
| `runLeft(int16_t pwm)` | Apply a signed left PWM command; zero brakes |
| `runRight(int16_t pwm)` | Apply a signed right PWM command; zero brakes |
| `setLeftInverted(bool inverted)` | Set left inversion; effective on the next run command |
| `setRightInverted(bool inverted)` | Set right inversion; effective on the next run command |
| `reverseLeft()` | Toggle left inversion on every call |
| `reverseRight()` | Toggle right inversion on every call |
| `beginLineSensors(s0, s1, s2, s3, inputPin)` | Initialize selectors and ADC pin; all arguments have defaults |
| `readLineSensor(uint8_t index)` | Return a raw sensor reading for 0–11, or `-1` for an invalid index |
| `setAdcResolution(uint8_t bits)` | Set the shared ADC return width, clamped to 1–15 |
| `linebegin(s0, s1, s2, s3, inputPin)` | Compatibility alias for `beginLineSensors()` with the same defaults |
| `linegetState(uint8_t index)` | Compatibility alias for `readLineSensor()` |

## Examples and repository structure

The two basic examples are included as Arduino sketches. The complete inversion
example is also shown above.

```text
PerseusS3/
├── src/
│   ├── PerseusS3.cpp
│   └── PerseusS3.h
├── examples/
│   ├── BasicMotorControl/
│   │   └── BasicMotorControl.ino
│   └── BasicLineSensor/
│       └── BasicLineSensor.ino
├── library.properties
├── README.md
└── LICENSE.md
```

The README image is loaded from the repository's `Images/PerseusForge.png`.
It is not required to compile or use the library.

## License

Copyright 2026 Davit Lanjuni.

Licensed under the [Apache License 2.0](LICENSE.md).
