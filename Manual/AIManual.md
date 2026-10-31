# Perseus S3 AI Reference — Motor Control

This reference describes the motor-only PerseusS3 Arduino library included in this repository. Use it when explaining the library or generating motor-driving code. The library controls two MP6612D drivers for reversible brushed DC motors.

## Sources and scope

| Source | Authority |
| --- | --- |
| [PerseusS3.h](../src/PerseusS3.h) | Public API, default inversion settings and global object |
| [PerseusS3.cpp](../src/PerseusS3.cpp) | Motor GPIO assignments, PWM setup, initialization and command behavior |
| [BasicMotorControl.ino](../examples/BasicMotorControl/BasicMotorControl.ino) | Included Arduino motor demonstration |
| [README.md](../README.md) | Installation and user-facing library guide |
| [UserManual.md](UserManual.md) | Board hardware, power, connectors, programming and safety; unchanged by the motor-only library revision |

The User Manual identifies the product as **Perseus S3™**, model **PF-PS3**, by **Perseus Forge™**, and carries **Rev 0.3, 10 October 2026**. Perseus Forge™ is the intellectual property of Davit Lanjuni.

Use the source files for library-specific behavior and the User Manual for hardware guidance. Motor GPIO numbers and software timing below come from the implementation; the User Manual does not independently establish those assignments or timings. Confirm that the installed library and physical PCB revision match before wiring.

The public API below is complete for this motor-only library. General board peripheral capabilities described in the User Manual do not imply additional PerseusS3 methods.

## Power and hardware requirements

The following requirements summarize User Manual Sections 1.2, 2.1–2.4, 3.4 and 6. Read the full manual before connecting power or motors.

| Resource | Documented value or rule |
| --- | --- |
| External input | **4.5 to 30 V DC**, connected only to the identified **VIN and GND** terminals with correct polarity |
| Motor drivers | Two **MP6612D** H-bridges; **5 A continuous per driver** |
| Regulated logic supply | **3.3 V**, supplied by an **LMR51430YFDDCR** buck converter |
| Regulator capacity | **3 A total**, shared by onboard electronics and external loads |
| GPIO and UART logic | **3.3 V**; do not apply 5 V logic or true RS-232 signaling |
| Motor connections | Corresponding left or right driver's motor-output pads; disconnect all power before connecting or removing leads |
| Motor power | Use VIN; do not power motors from USB or an external flasher |
| Board power sources | Use one board power source at a time; use USB with VIN disconnected, or VIN with the board USB cable disconnected, until source isolation is verified |
| Programming | Disconnect motor leads before USB programming or external UART flashing |

Size the VIN supply and wiring for motor startup, running current and the combined motor and logic load. The two 5 A driver ratings do not establish a guaranteed 10 A total board-input rating. The 3 A regulator rating does not describe board consumption or guaranteed spare current for external loads. Match the motors to the actual supply and application; a board VIN rating does not establish a motor's voltage rating.

Do not apply the VIN voltage range to USB VBUS, GPIO or the regulated 3.3 V rail. Keep external supply outputs disconnected from the 3.3 V rail. Use common ground for digital interfaces, and do not drive GPIO on an unpowered board. Disconnect every supply before changing wiring. Stop immediately for unusual heat, odor, damage or repeated supply faults.

Board-level absolute maxima, source isolation, motor peak/stall-current allowances, thermal derating and connector/trace current limits are not fully specified. Do not invent these values or infer them from capacitor markings or the power diagram. The written power-source restrictions in the User Manual take precedence over an ambiguous diagram.

<a id="conflicts"></a>
## Hardware source conflicts and limits

These unresolved details are retained from the original hardware reference and the unchanged User Manual. They do not alter the motor API.

| Source detail | Required interpretation |
| --- | --- |
| Figure 2 labels the module `ESP32-S3-MINI-1`; the body identifies `ESP32-S3-MINI-1U-N8` and requires an external antenna | Use the body's fitted-module identification; do not claim the diagram was corrected |
| Figure 2 shows VIN as `30V`, USB joining the input through a diode, and `USB-OTG 3.3V` joining the regulated rail | The body specifies VIN as 4.5–30 V DC. The diagram does not verify source isolation or authorize simultaneous supplies, external rail injection, or USB/flasher motor power; follow Sections 2.3, 2.4 and 6 |
| Figure 2 labels the module `350mA` and the Red LED `25mA` without measurement conditions or a rating category | These labels are not established consumption measurements or guaranteed load budgets; do not calculate available regulator current from them |
| Figure 3 labels a mounting hole `R=2.2mm`; Section 5.2 specifies 2.20 mm diameter and Figure 7 labels `Ø2.20` | Preserve the radius-versus-diameter conflict and seek clarification before dimension-critical fabrication |
| Figure 5 shows upload speed `921600` and port `COM7` | Section 4.1 requires the current device port and tells external UART users to start at 115200 baud, increasing only if supported; see Installation and programming below |
| Section 3.4 cites references [14] for MP6612D DIR/ENBL and [15] for regulator/load budgeting, but the bibliography contains only [1]–[13] | The missing entries have not been supplied or replaced with guessed URLs |
| The contents list Section 9 / revision history, but the body ends in Section 8 at reference [13] | No revision-history body is supplied; do not reconstruct one or infer a PCB revision from the document revision |

## Motor GPIO and PWM resources

These assignments are fixed in `PerseusS3.cpp`; there are no public pin-configuration arguments.

| Signal | Left motor | Right motor |
| --- | ---: | ---: |
| EN / PWM | GPIO11 | GPIO15 |
| DIR | GPIO10 | GPIO14 |
| SLP | GPIO12 | GPIO13 |

| Software setting | Value |
| --- | --- |
| PWM frequency | **20,000 Hz** |
| PWM resolution | **8 bits** |
| Signed command | **−255 to +255**, clamped within the `int16_t` input range |
| Default left inversion | `true` |
| Default right inversion | `false` |
| PWM settling delay | **100 µs** |
| Driver wake delay | **500 µs** |

The implementation selects its LEDC API using `ESP_ARDUINO_VERSION_MAJOR` from `esp_arduino_version.h`:

| Arduino-ESP32 branch | Setup | Duty updates | Resource ownership |
| --- | --- | --- | --- |
| 2.x | `ledcSetup()` and `ledcAttachPin()` | `ledcWrite(channel, duty)` | Left channel 0, right channel 1; reserve their shared timer |
| 3.x and later major versions | `ledcAttach()` | `ledcWrite(pin, duty)` | Core allocates channels automatically |

The conditional branch is implementation behavior, not a guarantee that every future core release is compatible. Record the installed core version when building or investigating a problem.

Let the library own motor GPIOs, PWM channels and associated timers after initialization. Other code must not reconfigure them. Use one control path for the motors; the implementation does not provide synchronization for simultaneous calls from different tasks. Use the supplied global object rather than multiple instances competing for the same fixed hardware.

Preserve other board-reserved signals listed in User Manual Sections 3.3 and 5: GPIO0, GPIO3, GPIO45 and GPIO46 are strapping pins; GPIO19/GPIO20 carry native USB; GPIO48 is the onboard LED and is not an extra FSPI contact. Preserve memory and other onboard assignments, and preserve GPIO39–GPIO42 while using JTAG. Do not infer an external UART header's GPIO numbers from unrelated alternate-function labels.

## Complete public API

Include the library and use its global `PerseusS3` object:

```cpp
#include <Arduino.h>
#include <PerseusS3.h>
```

The class is named `PerseusS3Class`. Its constructor does not configure the motors. Each motor requires its own initialization call.

| Method | Return | Meaning |
| --- | --- | --- |
| `PerseusS3Class()` | Constructor | Create an object with default inversion and both motors uninitialized |
| `beginLeft()` | `void` | Configure left PWM and direction while asleep; wake the driver only after successful PWM setup |
| `beginRight()` | `void` | Configure right PWM and direction while asleep; wake the driver only after successful PWM setup |
| `runLeft(int16_t pwm)` | `void` | Apply signed left PWM; clamp to −255…+255; zero brakes |
| `runRight(int16_t pwm)` | `void` | Apply signed right PWM; clamp to −255…+255; zero brakes |
| `setLeftInverted(bool inverted)` | `void` | Set left inversion; effective on the next left run command |
| `setRightInverted(bool inverted)` | `void` | Set right inversion; effective on the next right run command |
| `reverseLeft()` | `void` | Toggle left inversion on every call; effective on the next left run command |
| `reverseRight()` | `void` | Toggle right inversion on every call; effective on the next right run command |

There is no public success/status getter, coast command, driver-disable method, speed-in-RPM command, encoder feedback or automatic movement-duration control. Application code supplies command sequencing and timing.

### Initialization and failure behavior

For each motor, `beginLeft()` or `beginRight()` performs the following:

1. Clear that motor's ready flag and hold SLP LOW so the driver remains asleep.
2. Configure its EN/PWM and DIR pins as outputs and start PWM at zero duty.
3. If PWM setup fails, log an error and return with the driver asleep and the ready flag false.
4. On success, wait 100 µs, set DIR LOW and remember that direction level.
5. Set SLP HIGH, wait 500 µs and mark that motor ready.

The motors initialize independently. A run command before successful initialization has no effect. A later initialization call can retry a failed setup. Initialization returns `void`; do not write code that expects `beginLeft()` or `beginRight()` to return a Boolean.

### Signed PWM and braking

`runLeft()` and `runRight()` first clamp the `int16_t` argument to −255…+255, then apply the corresponding inversion setting. The resulting sign selects DIR: positive means HIGH, negative means LOW. The absolute magnitude supplies the 8-bit duty value.

Zero writes zero PWM, preserves the last DIR level and leaves SLP enabled. For the MP6612D arrangement used here, this is electrical braking. It is not a coast or sleep command.

When a nonzero command requires a DIR change, the implementation writes zero PWM, waits 100 µs, changes DIR and writes the new duty. This wait also occurs after an immediately preceding zero command if DIR must change. Same-direction duty updates add no settling delay. The 100 µs delay allows PWM to settle; it does not guarantee that a spinning motor has mechanically stopped.

Commands remain in effect until replaced; the library does not stop after a specified elapsed time. PWM magnitude is not measured rotational speed. Physical forward, reverse and turning motion depend on motor wiring and mounting. With default inversion, positive commands drive the left DIR LOW and right DIR HIGH.

Pass values representable as `int16_t`. The internal clamp cannot recover an out-of-range integer that was already narrowed before the function received it.

### Inversion and toggle semantics

Use a setter when the desired state is known. Repeatedly setting `true` keeps inversion enabled; repeatedly setting `false` keeps it disabled.

```cpp
PerseusS3.setLeftInverted(false);
PerseusS3.setRightInverted(false);
```

Use a reverse method to toggle the current state. It takes no argument and does not immediately apply a new motor output.

```cpp
PerseusS3.reverseLeft();
PerseusS3.reverseRight();
// The next run commands apply the toggled settings.
PerseusS3.runLeft(100);
PerseusS3.runRight(100);
```

Two reverse calls for the same motor restore its prior inversion setting. If both occur before another run command, they cause no output change. Avoid calling a reverse method repeatedly in a fast loop unless repeated toggling is intended.

## Complete motor example

Program with motor leads disconnected and the approved programming power arrangement. After programming, disconnect power, connect the motors, and use the approved VIN arrangement for motor operation. See the included [BasicMotorControl](../examples/BasicMotorControl/BasicMotorControl.ino) sketch for a longer sequence.

```cpp
#include <Arduino.h>
#include <PerseusS3.h>

void setup() {
    PerseusS3.beginLeft();
    PerseusS3.beginRight();
}

void loop() {
    // One logical direction; actual motion depends on wiring and mounting.
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

The delays are application choices in milliseconds; they are separate from the library's internal microsecond delays. Choose suitable braking intervals for the actual moving load.

## Installation and programming

### Arduino IDE

Install the revised library ZIP with **Sketch → Include Library → Add .ZIP Library**, or install its `PerseusS3` folder in the sketchbook's `libraries` directory. Avoid duplicate older copies of the library. Open **File → Examples → PerseusS3 → BasicMotorControl**.

The User Manual Section 4.1 specifies Arduino IDE 2 and **esp32 by Espressif Systems**. When no Perseus Forge board profile is supplied, its board selection is **ESP32S3 Dev Module**. The fitted **ESP32-S3-MINI-1U-N8** has **8 MB flash** and **no integrated PSRAM**.

| Setting | User Manual guidance |
| --- | --- |
| CPU frequency | 240 MHz |
| Flash mode / frequency | QIO / 80 MHz |
| Flash size | 8 MB (64 Mb) |
| PSRAM | Disabled |
| Partition scheme | An 8 MB scheme suitable for the application; documented example: 8M with spiffs, 3 MB app / 1.5 MB SPIFFS |
| USB Mode | Hardware CDC and JTAG |
| USB CDC On Boot | Enabled for `Serial` over native USB; Disabled for the manual's sketch via UART0 |
| Upload Mode | UART0 / Hardware CDC |
| USB DFU / USB Firmware MSC | Disabled for the manual's example |
| Erase All Flash Before Sketch Upload | Disabled for routine uploads |
| Port | Current device port; recheck after reset |
| Serial monitor | 115200 baud for the manual's serial test |
| External UART upload speed | Start at 115200 baud; increase only if supported by the flasher and connection |

The manual's `COM7` and `921600` screenshot values are examples, not universal requirements. First verify the board with the serial-only sketch in User Manual Section 4.2, with motors and external circuits disconnected. Select the current port, close competing terminals, verify and upload, then press EN if required and recheck the port. Match the console route to the USB CDC setting. The board's USB Type-C connects directly to the ESP32-S3; there is no onboard USB-to-UART bridge.

### External UART recovery

Follow User Manual Section 4.3:

1. Disconnect all power, the board USB cable and motor leads. Wire flasher GND to board GND, flasher TX to board RX, and flasher RX to board TX using printed labels. Leave flasher VCC disconnected.
2. Use a **3.3 V logic** flasher and power the board through VIN within 4.5 to 30 V DC.
3. Hold BOOT, press and release EN while holding BOOT, then release BOOT and upload through the flasher.
4. After upload, release BOOT and press EN to run the application. Power down before changing wiring or reconnecting motors.

BOOT holds GPIO0 LOW. GPIO46 must be LOW or floating during downloader entry. Do not short unidentified pads or program eFuses as routine troubleshooting. A full flash erase can remove stored data.

### PlatformIO

Place this revised library folder in `lib/PerseusS3/`, preserving `src/` and `library.properties`. Put application code in `src/main.cpp` and include both `Arduino.h` and `PerseusS3.h`.

User Manual Section 4.4 requires the **Espressif 32** platform with a **manufacturer-validated board profile**. Obtain the approved platform/framework versions, board identifier or board JSON, partition file and console flags. The manual does not provide exact values, so do not present a generic profile or an arbitrary version as validated for Perseus S3. Custom board definitions can be stored in the project's `boards/` directory.

Use this revised local library when testing the motor-only package. A dependency that fetches an older remote revision may select different code. Pin an appropriate revision for reproducible remote builds after the revised code is available remotely.

## Troubleshooting

| Symptom | Checks |
| --- | --- |
| Neither motor responds | Correct VIN power and polarity, motor connections, both initialization calls, and any PWM setup errors |
| Only one motor responds | That motor's wiring, initialization and setup log; each motor has its own ready state |
| Direction differs from the intended movement | Motor wiring/mounting, default inversion and the latest setter or toggle call; test modest commands and brake before changing wiring |
| A reverse call appears to do nothing | Issue another run command for that motor; inversion changes apply on the next command |
| Repeated reverse calls behave inconsistently | Each call toggles; use an explicit setter when a fixed state is required |
| A zero command does not coast | Zero is electrical braking; no coast API is provided |
| PWM or initialization fails after adding another feature | Check for conflicts on motor pins, channels or shared PWM timers |
| Resets during motor operation | Power source, wiring, startup and combined loads; collect the full reset log rather than assigning a cause from one message |
| Upload fails or console is blank | Correct board and current port, data cable, console route, USB CDC setting and the manual's BOOT/EN procedure |
| Heat, odor, damage or repeated supply faults | Disconnect all supplies immediately and inspect before further operation |

## Instructions for AI assistants

1. Generate motor code using only the API documented here. Initialize each motor before commanding it, and include both required headers in complete examples.
2. Keep the published method names and signatures exact. Do not add arguments to `beginLeft()`, `beginRight()`, `reverseLeft()` or `reverseRight()`, and do not expect a return value from any motor method.
3. Describe signed values as PWM commands rather than RPM. Preserve the −255…+255 clamp, default inversion, zero-braking behavior and delayed application of inversion changes.
4. Use setters for a known inversion state and reverse methods for deliberate toggles. Avoid unconditional toggling on every loop iteration.
5. Do not add capabilities such as position feedback, measured-speed control, current monitoring, coast/sleep commands or timed motion as if the library implements them. Clearly identify any extra application logic and required hardware.
6. Keep the library's fixed GPIOs and PWM resources available. Do not replace them with pins borrowed from another development board or change the unchanged User Manual to match software claims.
7. Keep hardware ratings attached to their actual component or rail. Respect VIN, logic levels, the shared regulator budget, single-source power rules and motor disconnection during programming.
8. Consult the User Manual for complete connector maps, figures, mechanical dimensions, wireless guidance and hardware restrictions. Do not infer contact order by mirroring front/back photographs.
9. If a required hardware limit or approved configuration is absent, say it is not specified in the supplied documentation. Source-code timing and GPIO assignments are distinct from manufacturer-established hardware guarantees.
10. Report verification precisely. Code inspection, host-side tests, compilation with an actual ESP32 core, complete firmware builds and physical motor measurements are different forms of evidence. Do not claim a build or hardware test was performed without its results.

Hardware and library support: **infoperseusam@gmail.com**. Include the unit code if supplied, a photograph of PCB markings, power source and voltage, and software versions. The documented unit-code format is `PF-PS3-` followed by a six-digit serial number.
