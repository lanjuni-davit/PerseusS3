# PerseusS3

Arduino library for the **Perseus S3™ robotics development board** by
**Perseus Forge™**, model **PF-PS3**, based on the ESP32-S3.
This motor-only edition provides signed PWM commands, electrical braking,
direction inversion, and reverse toggles for the left and right motors.
The sensor-array interface and its example have been removed. Existing sketches
must remove sensor API calls before using this edition.

Hardware specifications and board setup below follow the **Perseus S3 User
Manual, Rev 0.3, 10 October 2026**, included as
[Manual/UserManual.md](Manual/UserManual.md). Section references refer to that
manual, which is unchanged in this edition. Library behavior and GPIO assignments
below describe the implementation in `src/`. The
[AI reference](Manual/AIManual.md) covers the same motor-only API.

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

### Power and connection precautions

- Apply **4.5 to 30 V DC only to VIN and GND**, observing polarity. That range
  does not apply to USB VBUS, GPIO or the regulated 3.3 V rail.
- Use **3.3 V digital logic** and a common ground. Do not apply 5 V logic or
  true RS-232 signals, and do not drive GPIO on an unpowered board.
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
| Serial Monitor | 115200 baud for the User Manual's serial test sketch |
| External UART upload speed | Start at 115200 baud; increase only if the flasher and connection support it |

The manual's screenshot shows `COM7` and upload speed `921600`; these do not
replace the current-port selection or the qualified external-UART recommendation.

For the first upload, disconnect motors and external circuits. Follow the power
rules above, verify the sketch, select the current port, close competing terminals
and upload. Press EN if a reset is required, then recheck the port. Match the USB
CDC setting to the console being monitored. Use the manual's serial-only sketch
for the initial board check before running the motor example. The motor example
does not print Serial output.

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

Install this edition from the supplied ZIP or local library folder. A dependency
on the original GitHub repository may fetch a different edition; avoid keeping
both copies installed while testing this one.

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

### Verification of this edition

GCC C++ syntax and type checks passed for both the 2.x and 3.x API branches
using test interfaces, including the library, included motor example and complete
motor sketches in this README and the AI reference. Checks also confirmed that
the removed APIs are unavailable, the motor implementation is unchanged, and
the User Manual is byte-for-byte identical to the original.

A full Arduino-ESP32 3.3.11 firmware build was attempted but blocked by a Windows
compiler startup error. These checks do not establish a complete firmware build
or physical motor operation.

## Motor control

### Motor GPIO assignments

The table below matches `src/PerseusS3.cpp`. **The board
manual does not list these motor GPIO numbers or confirm the SLP mapping.**
It identifies MP6612D DIR and ENBL controls and instructs users to let the supplied
motor library configure its signals. Confirm that the installed library and PCB
revision match before relying on this table; do not manually reassign the pins.
The timing, initialization, braking and inversion descriptions that follow are
implementation details of this library.

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
asleep, starts with zero duty, waits **100 µs** before setting DIR LOW, then wakes
the driver and waits **500 µs** before accepting run commands.

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

## API reference

Include `PerseusS3.h` and call these methods on the global `PerseusS3` object.
All methods return `void`. Initialize each motor before sending run commands;
commands sent before successful initialization are ignored.

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

## Examples and repository structure

The motor example is included as an Arduino sketch. The complete inversion
example is also shown above.

```text
PerseusS3/
├── Images/
├── Manual/
│   ├── AIManual.md
│   └── UserManual.md
├── src/
│   ├── PerseusS3.cpp
│   └── PerseusS3.h
├── examples/
│   └── BasicMotorControl/
│       └── BasicMotorControl.ino
├── library.properties
├── README.md
└── LICENSE.md
```

The `Images/` files support the unchanged User Manual and are included in this
edition. They are not required to compile or use the library.

## Hardware documentation and support

Hardware authority: **Perseus S3 User Manual, Rev 0.3, 10 October 2026**,
included as [Manual/UserManual.md](Manual/UserManual.md). The manual identifies the board as
**Perseus S3™**, model **PF-PS3**, by **Perseus Forge™**. Perseus Forge™ is
the intellectual property of Davit Lanjuni.

Support and the supplied motor-control library: **infoperseusam@gmail.com**.
Include the unit code if supplied, a photograph of PCB markings, power source
and voltage, and software versions. The documented unit-code format is
`PF-PS3-` followed by a six-digit serial number.

## License

Copyright 2026 Davit Lanjuni.

Licensed under the [Apache License 2.0](LICENSE.md).
