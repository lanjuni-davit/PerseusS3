# PerseusS3

Arduino library for the **Perseus S3™ robotics development board** by
**Perseus Forge™**, model **PF-PS3**, based on the ESP32-S3.
The supplied library documentation describes signed PWM motor control,
direction inversion, and a 12-channel analog line-sensor interface.

Hardware specifications and board setup below follow the **Perseus S3 User
Manual, Rev 0.3, 10 October 2026**, supplied as `PerseusS3Datasheet.docx`.
Section references refer to that manual. Library API descriptions, default GPIO
assignments, examples and reported build results are retained from the supplied
library README; the manual does not independently verify those implementation
details. The library source and physical hardware were not tested for this
documentation update.

## Hardware overview

| Specification | Perseus S3 |
|---|---|
| Brand / model | Perseus Forge™ / PF-PS3 |
| MCU module | ESP32-S3-MINI-1U-N8 |
| Documented CPU frequency setting | 240 MHz in the Arduino configuration |
| Flash | 8 MB (64 Mb) |
| Integrated PSRAM | None; keep PSRAM disabled |
| Wireless | 2.4 GHz Wi-Fi and Bluetooth 5.0 Low Energy (BLE); Bluetooth Classic is not supported |
| Antenna | Compatible external 2.4 GHz antenna required for the MINI-1U module |
| GPIO and UART logic level | 3.3 V |
| External board input / VIN | 4.5 to 30 V DC |
| Onboard regulator | LMR51430YFDDCR buck converter; regulated 3.3 V |
| Regulator output capacity | 3 A total, shared by onboard electronics and external 3.3 V loads |
| Motor channels | Two reversible brushed DC motor channels |
| Motor drivers | Two MP6612D H-bridge drivers |
| Motor-driver continuous current | 5 A per driver |
| USB connector | USB Type-C connected directly to ESP32-S3 native USB |
| External flashing interface | Separate UART flasher header; 3.3 V logic; no onboard USB-to-UART bridge |
| Onboard LED | GPIO48; not an FSPI header contact |
| Board outline | 80 × 27 mm |
| Overall assembled height | 11.5 mm, including components; not bare PCB thickness |
| Mounting-hole centre spacing | 74 × 21 mm |
| Mounting-hole diameter | 2.20 mm in Section 5.2 and Figure 7; see the conflicting Figure 3 label below |
| Documented development environments | Arduino IDE 2 with esp32 by Espressif Systems; optional PlatformIO with a manufacturer-validated profile |

Source: manual Features and contents, Sections 1.1–1.2, 2.1–2.4, 3.1–3.6,
4.1 and 5–5.2. The manual specifies a 240 MHz CPU setting; it does not state
the CPU architecture or establish a separate maximum-frequency rating.

The manual rates **each motor driver at 5 A continuous**. Select the supply,
wiring and connectors for motor startup, running current and the combined load,
and account for thermal conditions. The **3 A regulator rating is a shared
3.3 V output budget**, not an additional allowance entirely available to external
loads. Neither rating is constant board power consumption. The manual does not
provide a peak/stall-current rating or a verified overcurrent trip threshold.

**Mechanical source conflict:** Figure 3 labels a mounting hole `R=2.2mm`,
whereas Section 5.2 and Figure 7 specify a **2.20 mm diameter**. Confirm the
dimension with support before manufacturing a dimension-critical mount; the
conflict has not been silently corrected.

The **12 line-sensor channels, indexed 0–11**, are a feature described by the
supplied library README. The board manual does not document an onboard
line-sensor array, a multiplexer part or its wiring. Do not treat the library's
sensor interface as a confirmed onboard hardware feature.

### Power and connection precautions

- Apply **4.5 to 30 V DC only to VIN and GND**, observing polarity. That range
  does not apply to USB VBUS, GPIO or the regulated 3.3 V rail.
- Use **3.3 V digital logic** and a common ground. Do not apply 5 V logic or
  true RS-232 signals, and do not drive GPIO on an unpowered board. The manual
  does not give a numeric ADC input range; 3.3 V logic is not an ADC calibration
  or input-range specification.
- Use VIN for motor operation. **Do not power motors from USB or an external
  flasher. Disconnect motor leads before USB programming or UART flashing.**
- Use **one board power source at a time**. Until source isolation is verified,
  use USB with VIN disconnected, or VIN with the board USB cable disconnected.
- For an external UART flasher, use 3.3 V logic, connect GND to GND, flasher TX
  to board RX and flasher RX to board TX. Power the target through VIN with the
  board USB cable and motor leads disconnected. **Leave flasher VCC disconnected.**
- Keep external supply outputs disconnected from the 3.3 V rail. Disconnect
  every power source before changing wiring or reconnecting motors.
- Preserve boot, native USB, memory, LED and motor-library assignments. GPIO0,
  GPIO3, GPIO45 and GPIO46 are strapping pins; GPIO19/20 are USB D-/D+;
  GPIO48 controls the onboard LED. Follow actual PCB labels.
- Attach a compatible external antenna with power disconnected before using
  wireless functions. Keep the board dry, handle it by the edges on an antistatic
  surface, avoid conductive mounting contact and connector strain, and stop for
  unusual heat, odor, damage or repeated supply faults.
- Do not infer USB Power Delivery, charging, board host mode, high-speed USB or
  a higher supply voltage from the Type-C connector. Do not change eFuses or
  permanent security settings during routine setup.

Source: manual Sections 2.1–2.4, 3.3, 3.5–3.6, 4.3, 5–5.1 and 6.

## Installation

### Arduino IDE

1. Install **Arduino IDE 2**.
2. In Preferences, add this stable package URL to **Additional Boards Manager URLs**:

   ```text
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```

3. In Boards Manager, install **esp32 by Espressif Systems** and record its version.
4. Select **ESP32S3 Dev Module** when no Perseus Forge board profile is supplied.
   Apply the manual's settings below.
5. Download the library ZIP and use **Sketch > Include Library > Add .ZIP Library**.
6. Include the library in your sketch:

```cpp
#include <Arduino.h>
#include <PerseusS3.h>
```

| Setting | Selection from manual Section 4.1 |
|---|---|
| Board | ESP32S3 Dev Module when no manufacturer profile is supplied |
| CPU frequency | 240 MHz |
| Flash mode and frequency | QIO, 80 MHz |
| Flash size | 8 MB (64 Mb) |
| PSRAM | Disabled |
| Partition scheme | An 8 MB scheme matching the application; the example `8M with spiffs` reserves 3 MB for the application and 1.5 MB for SPIFFS |
| USB Mode | Hardware CDC and JTAG |
| USB CDC On Boot | Enabled for Serial over native USB; Disabled for the manual's sketch over UART0 via an external flasher |
| Upload Mode | UART0 / Hardware CDC |
| USB DFU and USB Firmware MSC | Disabled for the manual's example |
| Erase All Flash Before Sketch Upload | Disabled for routine uploads |
| Port | Current device port; recheck after reset |
| Serial Monitor | 115200 baud for the supplied examples |
| External UART upload speed | Start at 115200 baud; increase only if the flasher and connection support it |

The manual's screenshot shows `COM7` and upload speed `921600`; these do not
replace the current-port selection or the qualified external-UART recommendation.

For the first upload, disconnect motors and external circuits. Follow the power
rules above, verify the sketch, select the current port, close competing terminals
and upload. Press EN if a reset is required, then recheck the port. Match the USB
CDC setting to the console being monitored. Use the manual's serial-only sketch
for the initial board check before running motor or sensor examples.

When replacing an older installation, avoid keeping multiple copies of the
library in the Arduino libraries folder.

### External UART recovery

With the UART wiring and VIN power arrangement described above, hold BOOT,
press and release EN while BOOT remains held, then release BOOT and upload.
After upload, release BOOT and press EN to run the application. BOOT holds
GPIO0 low; GPIO46 must be low or floating during downloader entry. Do not short
unidentified pads or program eFuses to troubleshoot. A full flash erase may
remove stored data. Source: manual Section 4.3.

### PlatformIO

For a local installation, put the library folder at `lib/PerseusS3/` inside your
PlatformIO project. Keep its `src/` folder and `library.properties` intact.

Use the **Espressif 32** platform with a **manufacturer-validated board profile**,
as required by manual Section 4.4. Obtain the approved platform/framework
versions, board identifier or board JSON, partition file and console flags.
Custom board JSON definitions can be placed in the project's `boards/` directory.

The manual does not supply those exact configuration values, so this README
does not present a generic ESP32-S3 profile or a pinned platform version as a
validated Perseus S3 configuration. Confirm memory, console and upload settings
before substituting another profile. Put your sketch in `src/main.cpp`, include
both `Arduino.h` and `PerseusS3.h`, and use **115200 baud** for the monitor.

To fetch a copy from GitHub instead, add this to the environment:

```ini
lib_deps =
    https://github.com/lanjuni-davit/PerseusS3.git
```

Use a repository revision containing the implementation documented here. Avoid
selecting an older remote copy while testing a corrected local copy. Pin a commit
or tag when you need a reproducible dependency.

## ESP32 core compatibility

The following compatibility behavior is reported by the supplied library README.
The board manual does not specify library support for particular ESP32 core
versions, and the implementation was not inspected for this update.

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

### Reported library verification status

These results are carried over from the supplied README. They were not rerun
or independently confirmed during the datasheet-based documentation correction.

| Reported check | Reported result |
|---|---|
| Library and both basic examples with real ESP32-S3 GCC and core 2.0.17 | Compiled into object files |
| Direction-inversion example with core 2.0.17 | Compiled into an object file |
| 2.x and 3.x API branches using test interfaces | Compiler checks passed |
| Full Arduino-ESP32 3.x firmware build | Not yet verified |
| Complete local PlatformIO build | Blocked by a local Windows compiler subprocess error |
| Physical motor and sensor measurements | Still required |

The supplied README reports a workflow defining example builds for cores
2.0.17, 3.0.7 and 3.3.12, plus regression tests. The workflow was not inspected
for this update. Those workflow runs and runtime tests are not claimed as
completed by the checks above.

## Motor control

### Motor GPIO assignments reported by the library

The table below is retained from the supplied library README. **The board
manual does not list these motor GPIO numbers or confirm the SLP mapping.**
It identifies MP6612D DIR and ENBL controls and instructs users to let the supplied
motor library configure its signals. Confirm that the installed library and PCB
revision match before relying on this table; do not manually reassign the pins.
The timing, initialization, braking and inversion descriptions that follow are
also library-reported behavior, not behavior established by the board manual.

Connect motors to the output pads for the corresponding left or right driver
with all power disconnected. Use VIN and observe the power and current limits
in [Hardware overview](#hardware-overview). Source: manual Section 3.4.

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

This section describes the **external sensor/multiplexer arrangement in the
supplied library README**. The board manual does not identify a multiplexer
part, enable polarity, channel count or supported sensor circuit. Confirm the
exact external component and wiring before applying the E/EN instructions below;
they are conditional on the active-LOW multiplexer arrangement described here.

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

The board manual confirms that GPIO33–GPIO37 are exposed on the **FSPI header**
and that GPIO7 has the **ADC1_CH6** alternate function. This supports locating
the contacts; it does not verify the library's multiplexer connections. The
default selector assignment uses GPIO33–GPIO36, so those contacts must not be
assigned to an SPI/FSPI peripheral at the same time. The custom example below
also uses GPIO37. Preserve the manual's reserved signals and verify the actual
PCB labels. Source: manual Sections 3.3 and 5; the shared-pin restriction follows
from comparing that map with the library's assignments.

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

The return-width limits and nominal numeric ranges above are library-reported
behavior. The supplied board manual maps ADC channels but does not specify native
ADC resolution, input-voltage range, attenuation or calibration. A wider numeric
return value is not evidence of additional measurement accuracy or increased
sensor optical sensitivity.

The supplied README states that this method calls the core's
`analogReadResolution()` and affects other `analogRead()` calls too. Keep that
shared setting within the library's supported range while using
`readLineSensor()`. Confirm the implementation and compatible core version before
relying on the described scaling or clamping behavior.

## API reference

This API table is retained from the supplied library README. Function signatures
and behavior require the matching library implementation; they are not an API
listing from the board manual.

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
|
|
├── Manual/
│   ├── AiManual.md
│   └── UserManual.md
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

## Hardware documentation and support

Hardware authority: **Perseus S3 User Manual, Rev 0.3, 10 October 2026**,
supplied as `PerseusS3Datasheet.docx`. The manual identifies the board as
**Perseus S3™**, model **PF-PS3**, by **Perseus Forge™**. Perseus Forge™ is
the intellectual property of Davit Lanjuni.

Support and the supplied motor-control library: **infoperseusam@gmail.com**.
Include the unit code if supplied, a photograph of PCB markings, power source
and voltage, and software versions. The documented unit-code format is
`PF-PS3-` followed by a six-digit serial number.

No external sources were consulted for this update. Where the manual does not
confirm a library-specific claim, that distinction is stated above. The
repository layout, dependency URL and software license are retained from
the supplied README; the board manual does not verify them or grant a software
license.

## License

Copyright 2026 Davit Lanjuni.

Licensed under the [Apache License 2.0](LICENSE.md).
