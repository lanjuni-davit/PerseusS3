> **Conversion note (not source text).** This Markdown preserves the supplied Word document’s wording and source order. Image transcriptions and conversion notes are explicitly labelled additions. All seven original image assets are embedded, byte-for-byte, as data-URI image references at the end of this file; no companion files or network connection are needed to retain those assets. Some Markdown viewers block data-URI images; the adjacent text records remain readable. Images, line wrapping, pagination, tab stops, repeated headers/footers, Word cropping/rotation and precise drawing geometry may display differently. The source header and footer are reproduced once. Source inconsistencies and omissions have not been silently repaired.

<!-- SOURCE HEADER -->
PERSEUS FORGE™　Perseus S3™

<!-- SOURCE B000 -->
# Perseus S3 User Manual

<!-- SOURCE B001 -->
ESP32 S3 Development Board

<!-- SOURCE B002 -->
Perseus Forge™

![Perseus S3 front view][source-image-1]

<details>
<summary>Image 1: labelled transcription and conversion observations</summary>

### Figure 1 — Perseus S3 front view

**Conversion transcription and visual description; not additional manufacturer prose.** The original is a photograph of the populated purple PCB. In the front-view orientation used elsewhere in the manual, the long GPIO header runs along the top, the module is central, USB is at the bottom, motor-driver circuits and paired output pads are at the left and right, and four corner mounting holes are visible. Two pushbuttons flank the module. An external antenna connector is visible on the module. The UART header and its printed `3.3`, `GND`, `RX`, `TX` labels are at the upper right; a five-contact header and `IO48` marking are below and to the right of the module. These are visual locations, not a new wiring authorization.

Readable prominent markings include `PERSEUS FORGE`, `Made on Earth for all beings`, `UART`, `U1`, `IO48`, `ESPRESSIF`, `ESP32-S3-MINI-1U`, `CE`, `CMIIT ID:2022DP8901(M)`, `MCN8`, and the module's Chinese company-name marking `乐鑫信息科技（上海）股份有限公司` (line breaks on the shield). A square machine-readable code is present; its payload was not decoded. The two large capacitor tops show `ROQANG`, `47`, `50V`, and `RVT`, with nearby `+` polarity marks. Small passive-component markings include visible `0`, `100`, `221`, and `132`; not every tiny marking is legible enough for a reliable transcription. These photographic component markings do not establish complete-board voltage or current limits. In particular, `50V` is a capacitor marking, not the permitted VIN voltage. Faint driver-package laser text cannot be reliably transcribed from the supplied photograph; the body's identified driver part is MP6612D.

The original image is preserved without changing its pixels in the human Markdown. Photographic microprint, the undecoded code, and exact placement cannot be fully replaced by this text description.

**Original Word alternative text:** Perseus S3 hardware illustration. See the adjacent caption and any review note for limits or corrections.

</details>

<!-- SOURCE B004 -->
Figure 1 Perseus S3 front view

<!-- SOURCE B005 -->
### Description

<!-- SOURCE B006 -->
### Perseus S3™ is an ESP32-S3-based development board designed by Perseus Forge™ for robotics prototyping, embedded systems education, and competitive robotics. Featuring integrated 2.4 GHz Wi-Fi and Bluetooth 5.0 Low Energy (BLE) connectivity, the board enables wireless communication, remote control, and IoT applications. This user manual provides essential information on board identification, power requirements, safety precautions, hardware interfaces, programming, and troubleshooting procedures.

<!-- SOURCE B007 -->
### Ownership notice

<!-- SOURCE B008 -->
Perseus Forge™ is the intellectual property of Davit Lanjuni. ©  
 2026 Davit Lanjuni. All rights reserved.

<!-- SOURCE B009 -->
Audience: students, educators, robotics developers and embedded-system engineers.  


<!-- SOURCE B010 -->
## Features and contents

<!-- SOURCE B011 -->
| Resource | Perseus S3 board specification | Details |
| --- | --- | --- |
| Processor and memory | ESP32-S3-MINI-1U-N8; 8 MB flash | 8 MB flash; no integrated PSRAM \[2\] |
| Board power input | 4.5 to 30 V DC | External board input |
| Voltage regulation | Onboard 3.3 V buck converter | 3 A total output capacity |
| Motor drivers | 2 x MP6612D H-bridge drivers | 5 A continuous per driver |
| Logic and connectivity | 3.3 V logic; USB Type-C | Native USB; separate UART flasher header |
| Wireless | 2.4 GHz Wi-Fi and Bluetooth LE | External antenna connection |

<!-- SOURCE B013 -->
### Contents

<!-- SOURCE B014 -->
| Section | Section number |
| --- | --- |
| The board and ratings | 1 and 2 |
| Power consumption and board power regulation | 2.2 to 2.4 |
| Functional overview and board topology | 3 and 3.1 |
| Processor and GPIO interfaces | 3.2 and 3.3 |
| Motor drivers and voltage regulation | 3.4 |
| Wireless and USB connectivity | 3.5 and 3.6 |
| Board operation and Arduino IDE setup | 4 and 4.1 |
| Sample sketch and recovery | 4.2 and 4.3 |
| PlatformIO and troubleshooting | 4.4 and 4.5 |
| Connector pinouts | 5 and 5.1 |
| Mechanical information and safety | 5.2 and 6 |
| Manufacturer information | 7 |
| References and revision history | 8 and 9 |

<!-- SOURCE B016 -->
## 1 The board

<!-- SOURCE B017 -->
### 1.1 Application examples and identification

<!-- SOURCE B018 -->
Use the board for serial programming exercises, connected data logging, robotics and embedded software prototypes. Check connector labels and electrical limits before attaching external circuits. Install a compatible external antenna before using wireless examples.

<!-- SOURCE B019 -->
Product identity: Perseus S3™ by Perseus Forge™. Model code: PF-PS3. The board includes an ESP32-S3-MINI-1U-N8 module, native USB Type-C, a separate UART flasher header, two MP6612D motor drivers and a 3.3 V buck converter.

<!-- SOURCE B020 -->
### 1.2 Board specifications

<!-- SOURCE B021 -->
Perseus S3 operates from a 4.5 to 30 V DC external input. Each of the two motor drivers supports 5 A continuous current. The onboard buck converter provides 3.3 V at up to 3 A in total, shared by onboard electronics and external loads. Supply, wiring, connector and thermal limits must support the combined load.

<!-- SOURCE B022 -->
## 2 Ratings

<!-- SOURCE B023 -->
### 2.1 Recommended operating conditions

<!-- SOURCE B024 -->
| Parameter | Board rating | Application |
| --- | --- | --- |
| Board input voltage/VIN | 4.5 to 30 V DC | Approved external power input |
| Motor-driver continuous current | 5 A per driver | Each MP6612D motor-driver circuit |
| Buck-converter output voltage | 3.3 V | Regulated board supply rail |
| Buck-converter output capacity | 3 A | Shared by onboard and external loads |
| Logic level | 3.3 V | GPIO and logic interfaces |

<!-- SOURCE B026 -->
CAUTION The 4.5 to 30 V range applies to the board DC power input. Do not apply this voltage to USB VBUS, GPIO or the 3.3 V rail. Connect only through the identified power input with the correct polarity.

<!-- SOURCE B027 -->
### 2.2 Power consumption

<!-- SOURCE B028 -->
Board input current depends on firmware, wireless activity, motor load and external peripherals. The 5 A rating is per motor driver; the 3 A rating is the total output capacity of the 3.3 V regulator. These ratings are not constant board consumption. Measure startup, idle, motor operation, radio and sleep conditions when selecting a supply \[12\].

<!-- SOURCE B029 -->
### 2.3 External power

<!-- SOURCE B030 -->
Connect 4.5 to 30 V DC to the terminals marked VIN and GND, observing polarity. Use an external supply and wiring suitable for the motors and the total board load. Do not run motors from USB or from an external flasher. Disconnect motor leads before USB programming or UART flashing. Until source isolation is verified, use USB power with VIN disconnected, or VIN power with the board  USB cable disconnected.

<!-- SOURCE B032 -->
### 2.4 Board power regulation

<!-- SOURCE B034 -->
| Connection | Electrical level | Purpose and limits |
| --- | --- | --- |
| VIN and GND | 4.5 to 30 V DC | External board and motor supply. Observe polarity; select the supply for the combined motor and logic load. |
| USB Type-C | USB power and native data | Direct ESP32-S3 programming and communication. Disconnect VIN and motor leads for USB programming. |
| UART flasher header | 3.3 V logic | External flasher TX/RX and common ground. Use VIN to power the target, with motors and the board USB cable disconnected; do not connect flasher VCC. |
| Regulated 3.3 V rail | 3 A total | LMR51430YFDDCR supplies onboard electronics and external 3.3 V loads. This is a shared current budget. |
| Motor drivers | 5 A per driver | Two MP6612D drivers. Use VIN for motor operation and follow the motor-control library instructions. |

![Power and programming connections][source-image-2]

<details>
<summary>Image 2: labelled transcription and conversion observations</summary>

### Figure 2 — Power and programming connections

**Conversion transcription and visual description; not additional manufacturer prose.** All block labels and numeric labels in the diagram are recorded below exactly as displayed (line grouping is represented by table columns).

| Block or rail | Displayed label or value |
| --- | --- |
| VIN | `30V` |
| USB | `5V` |
| USB-OTG | `3.3V` |
| LMR51430YFDDCR | `BUCK`, `3A` |
| Regulated output rail | `3V3` |
| MP6612D Left | `5A` |
| MP6612D Right | `5A` |
| ESP32-S3-MINI-1 | `350mA` |
| Red LED | `25mA` |

Drawn connections: VIN runs right to the buck-converter input and branches to the left and right motor-driver blocks. USB joins the VIN/buck input line through a diode symbol. The buck output feeds the `3V3` line, which branches to the module and Red LED blocks. The line labelled `USB-OTG` / `3.3V` joins the buck-output/3V3 line. This is a transcription of drawn connectivity, not an independently verified schematic or an instruction to connect supplies together.

**Unresolved source differences:** the body identifies the fitted module as ESP32-S3-MINI-1U-N8, while this figure says ESP32-S3-MINI-1. The figure provides only `30V` for VIN; the body's operating range is 4.5 to 30 V DC. The body does not classify `350mA` or `25mA` as measured consumption, typical values, maximum values, or guaranteed budgets. The meaning of the `USB-OTG` / `3.3V` input-like line and the isolation behavior are not explained. Sections 2.3, 2.4 and 6 explicitly restrict power-source use, prohibit powering motors from USB or a flasher, and keep flasher VCC disconnected. The drawn shared paths must not be used to override those written restrictions.

</details>

<!-- SOURCE B036 -->
Figure 2 Power and programming connections

<!-- SOURCE B037 -->
CAUTION The 3.3 V rail is the regulated logic supply. Do not connect an external flasher’s 5 V supply output to the board unless a separately approved power procedure specifies it. Disconnect every source before changing wiring. Stop immediately for unusual heat, odor, damage or repeated supply faults.

<!-- SOURCE B038 -->
## 3 Functional overview

<!-- SOURCE B039 -->
### 3.1 Board topology and components

<!-- SOURCE B040 -->
The board includes the ESP32-S3-MINI-1U-N8 module, a USB Type-C connector connected directly to the ESP32-S3, a separate UART header for an external flasher, two MP6612D motor drivers and an LMR51430YFDDCR buck converter providing 3.3 V at up to 3 A. Use the component maps for the PCB revision covered by this manual.

<!-- SOURCE B041 -->
| Component | Location and identification |
| --- | --- |
| Processor module | Central ESP32-S3-MINI-1U-N8 module. |
| BOOT and EN | Two buttons beside the module; BOOT selects downloader entry and EN resets the processor. |
| USB Type-C | Connector below the module when the long GPIO header is at the top. |
| VIN and GND | Two power terminals beside the USB area; follow the printed VIN/GND labels. |
| Motor drivers | MP6612D circuits at the left and right edges. |
| GPIO and peripheral headers | Long GPIO header at the top; I2C at the left, UART/JTAG at the right, FSPI near the power terminals. |
| Onboard LED | Controlled by GPIO48. GPIO48 is assigned to the LED rather than the FSPI header. |

![Front component locations][source-image-3]

<details>
<summary>Image 3: labelled transcription and conversion observations</summary>

### Figure 3 — Front component locations

**Conversion transcription and visual description; not additional manufacturer prose.** The photograph is annotated with these labels: `GPIO PINS`, `BOOT`, `ESP32-S3-MINI-1U`, `EN`, `I2C`, `UART`, `JTAG`, `DRIVER LEFT`, `DRIVER RIGHT`, `MOUNTING HOLE`, `R=2.2mm`, `BUCK CONVERTOR`, `USB`, `VIN`, and `FSPI`.

The long GPIO header is along the top. BOOT is left of the central module; EN is right of it. I2C is at the left edge above the left driver. UART is at the upper right, with JTAG below it and above the right driver. The buck converter is below-left of the module. USB is below the module. The pair of VIN-area terminals is below-right of the module. FSPI is the five-contact header just above those terminals. The mounting-hole arrow points to the lower-left corner hole. The underlying photo also carries the board branding and module markings described for Figure 1.

**Unresolved source conflict:** `R=2.2mm` is the exact mounting-hole annotation here. Section 5.2 calls the mounting-hole **diameter** 2.20 mm, and Figure 7 labels it `Ø2.20`. Do not silently convert the Figure 3 radius label into a diameter or select a machining dimension without clarification.

</details>

<!-- SOURCE B042 -->
Figure 3 Front component locations

![Perseus S3 back view][source-image-4]

<details>
<summary>Image 4: labelled transcription and conversion observations</summary>

### Figure 4 — Perseus S3 back view

**Conversion transcription and visual description; not additional manufacturer prose.** The photograph shows the purple reverse side, copper traces, soldered connector contacts, four corner mounting holes, two large power contacts, and a white decorative figure. With the long header at the top in this back-view photograph, the JTAG group is on the left and the I2C group is on the right; front and back views must not be interchanged when wiring.

Legible connector and power labels include `JTAG`; `39`, `40`, `41`, `42` beside that group; `IO37`, `IO36`, `IO35`, `IO34`, `IO33` at the central five-contact group; `VIN` and `GND` at the large power contacts; and `3V3`, `GND`, `IO8`, `IO9` at the right-hand group. Along the long header, the visible GPIO labels in the photograph's left-to-right direction include `IO18` (partly obscured by solder), `IO17`, `IO16`, `IO2`, `IO1`, `IO4`, `IO5`, `IO6`, and `IO7`, interspersed with `3V3` and `GND`. Some labels at overlapping contacts on the left are obscured; do not infer their missing characters or a physical contact number from this photograph. The source's front-view Figure 6 gives the clearer signal map.

**Original Word alternative text:** Perseus S3 hardware illustration. See the adjacent caption and any review note for limits or corrections.

</details>

<!-- SOURCE B043 -->
Figure 4  Perseus S3 back view

<!-- SOURCE B045 -->
| Identification item | Board implementation |
| --- | --- |
| Processor / USB | ESP32-S3-MINI-1U-N8; direct native USB Type-C; separate external-flasher UART |
| Motor drivers / regulator | 2 x MP6612D, 5 A continuous per driver; LMR51430YFDDCR, 3.3 V / 3 A total |
| Controls / antenna | BOOT and EN buttons; external antenna connector; onboard LED on GPIO48 |

<!-- SOURCE B046 -->
### 3.2 Processor

<!-- SOURCE B047 -->
The ESP32-S3-MINI-1U-N8 module runs the application firmware and provides wireless and peripheral functions. It has 8 MB of flash and no integrated PSRAM \[2\]. Use the corresponding settings in Section 4.1.

<!-- SOURCE B048 -->
Firmware, storage and update partitions share flash. Flash capacity is not runtime RAM, and the application partition is smaller than the complete flash device. Leave PSRAM disabled for this module. Changing partitions or erasing flash can remove stored data.

<!-- SOURCE B049 -->
### 3.3 GPIO and communication interfaces

<!-- SOURCE B050 -->
Use Figure 6 and the board labels to select GPIO and peripheral connections. Keep native USB, boot and onboard circuit assignments available to their functions. The supplied motor-control library configures the motor-driver signals.

<!-- SOURCE B051 -->
| Interface | Board function | Connection checks |
| --- | --- | --- |
| GPIO | Digital input and output | Exposed and reserved pins |
| UART | Serial communication | Routing, connector and direction |
| I2C | Two-wire peripheral bus | SDA/SCL routing and pull-ups |
| SPI | Synchronous peripheral bus | Clock/data/select mapping |
| I2S | Digital audio interface | Accessible signals |
| ADC | Analog input acquisition | Channels and input conditioning |
| LED PWM | PWM output for external loads | Permitted output pins |

<!-- SOURCE B053 -->
GPIO0, GPIO3, GPIO45 and GPIO46 are chip strapping pins. GPIO19 is USB D- and GPIO20 is USB D+. Reassigning those USB pins can disable the native connection \[3\]. GPIO48 controls the onboard LED. Do not disturb memory, motor-library or other onboard-reserved signals.

<!-- SOURCE B054 -->
### 3.4 Motor drivers and voltage regulation

<!-- SOURCE B055 -->
Perseus S3 includes two MP6612D H-bridge motor drivers for reversible brushed DC motors. Each driver supports 5 A continuous current. The MP6612D uses DIR and ENBL control inputs \[14\]. Operate the complete board within its 4.5 to 30 V DC external-input range.

<!-- SOURCE B056 -->
| Board function | Rating | Use |
| --- | --- | --- |
| DC power input | 4.5 to 30 V DC | Board power supply |
| Motor drivers | 2 x MP6612D | Reversible brushed DC motors |
| Motor-driver continuous current | 5 A per driver | Continuous motor loading |
| Buck-converter output | 3.3 V | Regulated logic supply |
| Buck-converter output capacity | 3 A | Shared by onboard and external loads |

<!-- SOURCE B057 -->
### Motor connections and control

<!-- SOURCE B058 -->
Connect motors to the motor-output pads for the corresponding left or right driver. Disconnect all power before connecting or removing a motor. Use the supplied motor-control library rather than assigning driver GPIOs manually. Select VIN supply capacity and wiring for motor startup, running current and the combined load of both drivers.

<!-- SOURCE B059 -->
### Onboard buck converter

<!-- SOURCE B060 -->
The fitted LMR51430YFDDCR buck converter provides a regulated 3.3 V supply with a verified total output capacity of 3 A. This capacity is shared by the board’s electronics and loads connected to the 3.3 V rail. Account for onboard consumption, connector and trace limits, and thermal conditions when budgeting external loads \[15\].

<!-- SOURCE B061 -->
Use the motor-control library supplied for Perseus S3. The library manages the motor-control GPIO assignments; use its documented commands and examples. Obtain the library and support from infoperseusam@gmail.com.

<!-- SOURCE B063 -->
### 3.5 Wireless connectivity

<!-- SOURCE B064 -->
Use a compatible 2.4 GHz Wi-Fi network and ESP32-S3 examples. Arduino-ESP32 supports station and access-point operation \[11\]. Bluetooth support is Bluetooth Low Energy; Bluetooth Classic is not supported \[1\]. Keep credentials out of shared source files and logs.

<!-- SOURCE B065 -->
The ESP32-S3-MINI-1U requires a compatible external 2.4 GHz antenna \[2\]. Use an antenna and mating connector intended for this module. With power disconnected, attach the antenna carefully without straining the connector or cable. Check applicable antenna and radio requirements before transmission.

<!-- SOURCE B066 -->
### 3.6 USB connectivity

<!-- SOURCE B067 -->
The USB Type-C port is connected directly to the ESP32-S3 native USB interface. Use it for native USB programming and communication. The UART header is a separate interface for an external flasher; there is no onboard USB-to-UART bridge. ESP32-S3 supports full-speed USB OTG and USB Serial/JTAG \[1\].

<!-- SOURCE B068 -->
| Interface | Programming and communication |
| --- | --- |
| USB Type-C | Direct ESP32-S3 native USB. Use the settings in Section 4.1. |
| Separate UART header | Connect an external flasher with compatible 3.3 V logic, verified pin order and common ground. |

<!-- SOURCE B070 -->
### Connection checks

<!-- SOURCE B071 -->
1. Use the USB Type-C port for native USB programming. Observe the power-source rules in Section 2.3.

<!-- SOURCE B072 -->
2. Use an undamaged data-capable cable for the approved programming port, without external wiring attached.

<!-- SOURCE B073 -->
3. Select the current USB device port. No onboard USB-to-UART bridge driver is required. An external UART flasher may require its own driver.

<!-- SOURCE B074 -->
CAUTION Do not infer USB Power Delivery, charging, host mode, high-speed operation or a higher supply voltage from the connector shape.  


<!-- SOURCE B075 -->
## 4 Board operation

<!-- SOURCE B076 -->
### 4.1 Arduino IDE installation and settings

<!-- SOURCE B077 -->
1. Install Arduino IDE 2 from the official software page \[13\].

<!-- SOURCE B078 -->
2.  In Preferences, add the stable Espressif package URL below to Additional Boards Manager URLs.

<!-- SOURCE B079 -->
3. In Boards Manager, install esp32 by Espressif Systems and record its version \[4\].

<!-- SOURCE B080 -->
4. Select ESP32S3 Dev Module when no Perseus Forge board profile is supplied \[5\]. Use 8 MB flash and PSRAM disabled for the fitted ESP32-S3-MINI-1U-N8 module \[2\].

<!-- SOURCE B081 -->
[https://espressif.github.io/arduino-esp32/package\_esp32\_index.json](https://espressif.github.io/arduino-esp32/package_esp32_index.json)

<!-- SOURCE B083 -->
For the native USB connection, use the settings in Figure 5. USB CDC On Boot must be Enabled for Serial output over USB. When using the separate UART flasher and UART console instead, set USB CDC On Boot to Disabled so this sketch uses UART0. Match the console setting to the interface you will monitor \[5, 6\].

<!-- SOURCE B084 -->
| Setting | Selection |
| --- | --- |
| Board | ESP32S3 Dev Module |
| CPU frequency | 240 MHz |
| Flash mode and frequency | QIO, 80 MHz |
| Flash size | 8 MB (64 Mb) |
| PSRAM | Disabled |
| Partition scheme | An 8 MB scheme matching the application. The example 8M with spiffs scheme reserves 3 MB for the app and 1.5 MB for SPIFFS. |
| USB Mode | Hardware CDC and JTAG |
| USB CDC On Boot | Enabled for the native USB Serial example |
| Upload Mode | UART0 / Hardware CDC |
| USB DFU and USB Firmware MSC | Disabled for this example |
| Erase All Flash Before Sketch Upload | Disabled for routine uploads |
| Port | The current device port; the port name can change after reset |
| Serial Monitor | 115200 baud |
| External UART upload speed | Start at 115200 baud; increase only if the flasher and connection support it |

![Arduino IDE configuration for the serial example][source-image-5]

<details>
<summary>Image 5: labelled transcription and conversion observations</summary>

### Figure 5 — Arduino IDE configuration for the serial example

**Conversion transcription.** The screenshot's menu text, in display order, is:

```text
Board: "ESP32S3 Dev Module"
Port: "COM7"
Reload Board Data
Get Board Info
USB CDC On Boot: "Enabled"
CPU Frequency: "240MHz (WiFi)"
Core Debug Level: "None"
USB DFU On Boot: "Disabled"
Erase All Flash Before Sketch Upload: "Disabled"
Events Run On: "Core 1"
Flash Mode: "QIO 80MHz"
Flash Size: "8MB (64Mb)"
JTAG Adapter: "Disabled"
Arduino Runs On: "Core 1"
USB Firmware MSC On Boot: "Disabled"
Partition Scheme: "8M with spiffs (3MB APP/1.5MB SPIFFS)"
PSRAM: "Disabled"
Upload Mode: "UART0 / Hardware CDC"
Upload Speed: "921600"
USB Mode: "Hardware CDC and JTAG"
```

**Context from Section 4.1:** `COM7` is the screenshot's displayed port; the settings table requires the current device port and warns that it can change after reset. The screenshot shows upload speed `921600`, whereas the table tells an external UART user to start at 115200 baud and increase only if the flasher and connection support it. Preserve both statements; the screenshot is not an unconditional external-UART speed requirement. Core Debug Level, Events Run On, JTAG Adapter, and Arduino Runs On are visible screenshot settings whose purpose or general validity is not explained further in the source.

</details>

<!-- SOURCE B103 -->
Figure 5 Arduino IDE configuration for the serial example  


<!-- SOURCE B104 -->
### 4.2 Sample sketch and first upload

<!-- SOURCE B105 -->
This serial example does not assign external GPIO. Disconnect motors and external circuits for the first upload. Use the native USB settings in Section 4.1.

<!-- SOURCE B106-B114 -->
```cpp
#include <Arduino.h>
void setup() {
  Serial.begin(115200);
  delay(1500);
}
void loop() {
  Serial.println("Perseus S3 running");
  delay(1000);
}
```

<!-- SOURCE B116 -->
1.  Save the sketch and apply the confirmed settings. Select Verify and resolve compilation errors.

<!-- SOURCE B117 -->
2.  Connect through the approved programming port. Select the current port and close competing terminals. Select Upload.

<!-- SOURCE B118 -->
3.  After the first native USB upload, press EN if a reset is required. Recheck the device port and open Serial Monitor at 115200 baud \[6\].

<!-- SOURCE B119 -->
Expected result: Perseus S3 running about once per second. Serial uses the configured console \[5, 6\]. Blank output requires checking the application port and routing before assuming upload failure.

<!-- SOURCE B120 -->
### 4.3 External UART flashing and recovery

<!-- SOURCE B121 -->
Use a USB-to-UART flasher with 3.3 V logic. BOOT holds GPIO0 low for downloader entry; EN resets the processor. GPIO46 must be low or floating during downloader entry \[7\]. Connect by the UART header’s printed TX, RX and GND labels; do not infer pin order from the adapter connector.

<!-- SOURCE B122 -->
1.  Disconnect all power, the board USB cable and motor leads. Connect flasher GND to board GND, flasher TX to board RX, and flasher RX to board TX. Leave the flasher’s power/VCC output disconnected.

<!-- SOURCE B123 -->
2.  Power the target through VIN within 4.5 to 30 V DC. Connect the external flasher to the computer, select its port and use 3.3 V UART logic.

<!-- SOURCE B124 -->
3.  Hold BOOT, press and release EN while BOOT remains held, then release BOOT. Upload through the external flasher. After upload, release BOOT and press EN to run the application.

<!-- SOURCE B125 -->
CAUTION  Do not short unidentified pads or change eFuses to troubleshoot. eFuse programming can be irreversible \[1\]. A full flash erase may remove stored data. Preserve it before an approved erase procedure.

<!-- SOURCE B126 -->
For the Section 4.2 sketch over UART0, set USB CDC On Boot = Disabled and monitor through the external flasher at 115200 baud. For the same sketch over native USB, use USB CDC On Boot = Enabled. Power down before changing wiring or reconnecting motors.

<!-- SOURCE B128 -->
### 4.4 Programming with PlatformIO

<!-- SOURCE B129 -->
PlatformIO is optional. Use the Espressif 32 platform and a manufacturer-validated profile \[8\]. Custom board JSON definitions can be stored in a project boards directory \[9\]. Do not substitute another board without checking memory, console and upload assumptions.

<!-- SOURCE B130 -->
1.  Obtain approved platform/framework versions, board identifier or JSON definition, partition file and console flags.

<!-- SOURCE B131 -->
2.  Create an Arduino project and place the Section 4.2 sketch in src/main.cpp. Review the verified memory and upload settings.

<!-- SOURCE B132 -->
3.  Build, upload through the approved port, then open the monitor at 115200 baud.

<!-- SOURCE B133 -->
### 4.5 Troubleshooting

<!-- SOURCE B134 -->
| Symptom | Check and corrective action |
| --- | --- |
| No USB device | Use the native USB Type-C port, an undamaged data cable and a working host port. Check board power and the device port. There is no onboard USB-to-UART bridge. |
| Upload failure | Select correct ESP32-S3 target and current port. Close other terminals; use the verified downloader procedure. |
| Stalled upload | Inspect cable and supply stability. For a separate external UART flasher, reduce the upload speed if needed \[5\]. |
| Blank console | Recheck port after reset, console route and USB CDC setting; use 115200 baud for the sample. |
| Repeated resets | Remove external wiring. Check power, boot straps and complete reset log; do not infer a cause from one message. |
| Wireless failure | Confirm antenna and 2.4 GHz network; check credentials and application status. |
| GPIO / bus failure | Check pin map, ground, voltage, restrictions and software assignment; do not guess alternative pins. |
| Heat or odor | Disconnect every supply immediately and inspect before reuse. |

<!-- SOURCE B137 -->
## 5 Connector pinouts

<!-- SOURCE B138 -->
Use Figure 6 and the PCB’s printed labels to identify signals. GPIO numbers, module pad numbers and physical connector pin numbers are different identifiers. View the front of the board with the long GPIO header at the top and USB Type-C at the bottom. For wiring, follow the signal labels on the actual connector; never infer a pin order from another ESP32 board.

![Connector groups and GPIO functions][source-image-6]

<details>
<summary>Image 6: labelled transcription and conversion observations</summary>

### Figure 6 — Connector groups and GPIO functions

**Conversion transcription.** View the front with the long GPIO header at the top and USB at the bottom. Each long-header signal below is followed, left-to-right in the drawing, by a contact labelled `3.3V` and then a contact labelled `GND`. The nine groups appear in the order shown. This is the drawing's order; verify the actual PCB labels before wiring, as required by Sections 5 and 5.1.

| Long-header signal label | Following contact | Following contact |
| --- | --- | --- |
| `GPIO 7 / ADC1_CH6 / TOUCH 7` | `3.3V` | `GND` |
| `GPIO 6 / ADC1_CH5 / TOUCH 6` | `3.3V` | `GND` |
| `GPIO 5 / ADC1_CH4 / TOUCH 5` | `3.3V` | `GND` |
| `GPIO 4 / ADC1_CH3 / TOUCH 4` | `3.3V` | `GND` |
| `GPIO 1 / ADC1_CH0 / TOUCH 1` | `3.3V` | `GND` |
| `GPIO 2 / ADC1_CH1 / TOUCH 2` | `3.3V` | `GND` |
| `GPIO 16 / ADC2_CH5 / U0CTS` | `3.3V` | `GND` |
| `GPIO 17 / ADC2_CH6 / U1TXD` | `3.3V` | `GND` |
| `GPIO 18 / ADC2_CH7 / U1RXD` | `3.3V` | `GND` |

| Left-hand four-contact group, left-to-right in the drawing | Exact function labels |
| --- | --- |
| First | `GPIO 9 / ADC1_CH8 / TOUCH 9 / SCL` |
| Second | `GPIO 8 / ADC1_CH7 / TOUCH 8 / SDA` |
| Third | `GND` |
| Fourth | `3.3V` |

| Five-contact group below/right of module, left-to-right | Exact function labels |
| --- | --- |
| GPIO33 | `GPIO 33 / FSPIHD / SPIIO4` |
| GPIO34 | `GPIO 34 / FSPICS0 / SPIIO5` |
| GPIO35 | `GPIO 35 / FSPID / SPIIO6` |
| GPIO36 | `GPIO 36 / FSPICLK / SPIIO7` |
| GPIO37 | `GPIO 37 / FSPIQ / SPIDQS` |

| Right-hand JTAG group, left-to-right | Exact function labels |
| --- | --- |
| GPIO39 | `GPIO 39 / TCK` |
| GPIO40 | `GPIO 40 / TDO` |
| GPIO41 | `GPIO 41 / TDI` |
| GPIO42 | `GPIO 42 / TMS` |

The exact warning in the image is:

> WARNING: JTAG PINS (GPIO39–42).
> Do not use unless you know what you’re doing.

The UART header is marked `UART`, with `3.3`, `GND`, `RX`, `TX` left-to-right in the front-view drawing; part of the GND lettering is cramped. No GPIO numbers are assigned to that UART header in the figure. Other visible PCB labels include `U4`, `IO48`, `PERSEUS FORGE`, and `Made on Earth for all beings`. `IO48` is adjacent to the onboard LED area; Sections 3.1 and 5 explicitly say GPIO48 is the LED signal, not an extra FSPI contact. The text table names the JTAG functions `MTCK`, `MTDO`, `MTDI`, `MTMS`; the figure abbreviates those names to `TCK`, `TDO`, `TDI`, `TMS`. Alternate-function labels do not by themselves establish an enabled firmware configuration or permission to repurpose reserved signals.

</details>

<!-- SOURCE B140 -->
Figure 6 Connector groups and GPIO functions

<!-- SOURCE B141 -->
| Connector group | Signals | Purpose |
| --- | --- | --- |
| Long GPIO header | GPIO7, GPIO6, GPIO5, GPIO4, GPIO1, GPIO2, GPIO16, GPIO17, GPIO18; 3.3 V and GND contacts | General GPIO and supported peripheral functions; use the PCB labels to identify individual contacts. |
| I2C header | GPIO9, GPIO8, GND and 3.3 V | GPIO9/SCL and GPIO8/SDA in the supplied map. Verify peripheral voltage and pull-ups. |
| FSPI header | GPIO33, GPIO34, GPIO35, GPIO36, GPIO37 | FSPIHD, FSPICS0, FSPID, FSPICLK and FSPIQ alternate functions, respectively. |
| JTAG header | GPIO39, GPIO40, GPIO41, GPIO42 | MTCK, MTDO, MTDI and MTMS, respectively. Preserve these assignments when using JTAG. |
| UART flasher header | TX, RX, GND and the labelled 3.3 V rail contact | External flasher: cross TX/RX and connect GND. Leave flasher VCC disconnected. |
| Onboard LED | GPIO48 | Built-in LED control; not an FSPI header signal. |

<!-- SOURCE B142 -->
The GPIO functions below use the ESP32-S3 pin-function table \[1\]. A listed alternate function must still be selected in firmware. The motor library owns its control signals, and GPIO48 is assigned to the onboard LED.

<!-- SOURCE B143 -->
| GPIO | ADC function | Touch function or alternate UART function |
| --- | --- | --- |
| GPIO1 | ADC1\_CH0 | TOUCH1 |
| GPIO2 | ADC1\_CH1 | TOUCH2 |
| GPIO4 | ADC1\_CH3 | TOUCH4 |
| GPIO5 | ADC1\_CH4 | TOUCH5 |
| GPIO6 | ADC1\_CH5 | TOUCH6 |
| GPIO7 | ADC1\_CH6 | TOUCH7 |
| GPIO8 | ADC1\_CH7 | TOUCH8 |
| GPIO9 | ADC1\_CH8 | TOUCH9 |
| GPIO16 | ADC2\_CH5 | U0CTS |
| GPIO17 | ADC2\_CH6 | U1TXD |
| GPIO18 | ADC2\_CH7 | U1RXD |

<!-- SOURCE B144 -->
Use 3.3 V logic and a common ground for every digital interface. Do not drive GPIO on an unpowered board. GPIO48 is the onboard LED signal and must not be treated as an extra FSPI header contact.

<!-- SOURCE B145 -->
### 5.1 Connector records

<!-- SOURCE B146 -->
Check connector labels, signal direction, voltage limits and reserved functions before connection. Keep boot, native USB, memory, LED and motor-library assignments available to their onboard functions. For a board whose connector labels differ from this manual, contact support before wiring.

<!-- SOURCE B147 -->
CAUTION  Do not wire by similarity to another development board. Confirm common ground and 3.3 V compatibility. Do not drive an unpowered board unless its power-off tolerance is verified.

<!-- SOURCE B149 -->
### 5.2 Mechanical information

<!-- SOURCE B150 -->
The board outline is 80 x 27 mm. The mounting-hole centre spacing is 74 x 21 mm, and the mounting-hole diameter is 2.20 mm. Overall assembled height, including components, is 11.5 mm. The 11.5 mm value is not the thickness of the bare PCB. Use nonconductive mounting clearances to avoid contact with circuitry.

![Board dimensions in millimetres][source-image-7]

<details>
<summary>Image 7: labelled transcription and conversion observations</summary>

### Figure 7 — Board dimensions in millimetres

**Conversion transcription and visual description.** The drawing contains a top view and a side view. All dimensions are in millimetres, as stated by the source caption. Decimal precision and radius/diameter symbols below follow the visible labels. Feature descriptions identify where each label appears in the drawing; they do not add manufacturing tolerances.

| View | Exact dimension label | Visible dimension context |
| --- | --- | --- |
| Top | `74.00` | Horizontal centre spacing between corner mounting holes |
| Top | `21.00` | Vertical centre spacing between corner mounting holes |
| Top | `80.00` | Overall board outline length |
| Top | `27.00` | Overall board outline width |
| Top | `Ø2.20` | Corner mounting-hole diameter |
| Top | `R3.00` | Rounded corner radius |
| Top | `Ø1.00` | Leader to a small long-header hole |
| Top | `2.54` | Spacing between adjacent long-header holes |
| Top | `Ø2.70` | Diameter callout at a large power-contact hole |
| Top | `5.00` | Spacing between the two large power-contact holes |
| Top | `6.80` | Leader and vertical dimension near the USB-side component/connector area; the feature is not named in the source |
| Side | `11.50` | Overall assembled height |
| Side | `1.51` | Vertical dimension at the right-hand board edge, spanning the drawn bare board thickness |
| Side | `6.30` | Width across the left tall component |
| Side | `51.40` | Horizontal distance between the inner faces of the two tall components |
| Side | `7.40` | Left board edge to the left face of the left tall component |
| Side | `8.60` | Right face of the right tall component to the right board edge |
| Side | `8.94` | Width across the USB connector in the side view |
| Side | `3.16` | Vertical USB-area callout; a leader points to its dimension line |

Section 5.2 explicitly says the 11.5 mm value is assembled height, not bare PCB thickness. No dimensional tolerances are given. The exact graphical endpoints of the `6.80` and `3.16` callouts are not sufficiently explained by source text to use this transcription as a fabrication drawing. Preserve those values without assigning an undocumented feature name. Figure 3's `R=2.2mm` mounting-hole label conflicts with `Ø2.20` here; seek clarification for machining or clearance-critical use.

**Original Word alternative text:** Perseus S3 hardware illustration. See the adjacent caption and any review note for limits or corrections.

</details>

<!-- SOURCE B152 -->
Figure 7 Board dimensions in millimetres

<!-- SOURCE B153 -->
## 6 Safety precautions

<!-- SOURCE B154 -->
•  Use VIN within 4.5 to 30 V DC for motor operation. Do not power motors from USB or an external flasher. Disconnect all sources before rewiring, check polarity and remove loose conductive objects.

<!-- SOURCE B155 -->
•  Use 3.3 V logic for GPIO and UART signals. Do not apply 5 V logic or use a true RS-232 voltage interface. Keep external supply outputs disconnected from the 3.3 V rail and use only one board power source at a time.

<!-- SOURCE B156 -->
•  Handle by the edges on an antistatic surface. Keep the board dry; prevent conductive mounting parts from touching circuitry.

<!-- SOURCE B157 -->
•  Do not strain USB or antenna connectors. Confirm antenna installation before transmission. Stop immediately for excessive heat, odor or damage.

<!-- SOURCE B158 -->
•  Use the supplied motor-control library, observe reserved GPIO assignments and retain a working recovery sketch. Do not program eFuses or permanent security settings during routine setup.

<!-- SOURCE B159 -->
## 7 Manufacturer information

<!-- SOURCE B160 -->
Brand: Perseus Forge™. Product: Perseus S3™. Perseus Forge™ is the intellectual property of Davit Lanjuni. © 2026 Davit Lanjuni. All rights reserved.

<!-- SOURCE B161 -->
| Identifier | Value |
| --- | --- |
| Brand | Perseus Forge™ |
| Product | Perseus S3™ |
| Model code | PF-PS3 |
| Unit-code format | PF-PS3- followed by a six-digit serial number |
| Support email | infoperseusam@gmail.com |

<!-- SOURCE B162 -->
Support: infoperseusam@gmail.com. When requesting help, include the unit code if supplied, a photograph of the PCB markings, the power source and voltage, and the software versions used.

<!-- SOURCE B163 -->
## 8 Reference documentation

<!-- SOURCE B164 -->
The following official component and software references support the functional descriptions in this manual. Complete-board ratings are stated in Sections 2.1 and 3.4. Component maximum ratings do not replace the board operating limits. References were checked on 10 October 2026.

<!-- SOURCE B165 -->
[\[1\] ESP32-S3 Series Datasheet v2.2](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)

<!-- SOURCE B166 -->
[\[2\] MINI-1 and MINI-1U Datasheet v1.7](https://documentation.espressif.com/esp32-s3-mini-1_mini-1u_datasheet_en.html)

<!-- SOURCE B167 -->
[\[3\] ESP-IDF GPIO and RTC GPIO](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/gpio.html)

<!-- SOURCE B168 -->
[\[4\] Arduino-ESP32 installation](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)

<!-- SOURCE B169 -->
[\[5\] Arduino-ESP32 Tools Menu](https://docs.espressif.com/projects/arduino-esp32/en/latest/guides/tools_menu.html)

<!-- SOURCE B170 -->
[\[6\] Arduino-ESP32 USB CDC and DFU](https://docs.espressif.com/projects/arduino-esp32/en/latest/tutorials/cdc_dfu_flash.html)

<!-- SOURCE B171 -->
[\[7\] esptool ESP32-S3 boot mode selection](https://docs.espressif.com/projects/esptool/en/latest/esp32s3/advanced-topics/boot-mode-selection.html)

<!-- SOURCE B172 -->
[\[8\] PlatformIO Espressif 32](https://docs.platformio.org/en/latest/platforms/espressif32.html)

<!-- SOURCE B173 -->
[\[9\] PlatformIO custom embedded boards](https://docs.platformio.org/en/latest/platforms/creating_board.html)

<!-- SOURCE B174 -->
[\[10\] Arduino-ESP32 ADC API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html)

<!-- SOURCE B175 -->
[\[11\] Arduino-ESP32 Wi-Fi API](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html)

<!-- SOURCE B176 -->
[\[12\] ESP-IDF ESP32-S3 sleep modes](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/system/sleep_modes.html)

<!-- SOURCE B177 -->
[\[13\] Arduino Software](https://www.arduino.cc/en/software)

## Source footer (reproduced once)

<!-- SOURCE FOOTER -->
1 / 1　Perseus S3™ User Manual　Rev 0.3  \|  10 October 2026

© 2026 Davit Lanjuni. All rights reserved.

## Conversion verification and limitations (not source text)

- The complete package was inspected: all 127 nonempty body paragraphs, all 14 Word tables (111 rows including their header rows), all 7 embedded images, the source header/footer, hyperlinks, and available metadata. All 14 external link targets were retained without opening them. No external information was added.
- The body ends with reference [13]. The contents table mentions Section 9 / revision history, but no Section 9 body exists. References [14] and [15] are cited in Sections 3.4 but their bibliographic entries are absent. They have not been fabricated.
- No substantive footnotes, endnotes, comments, native equations, tracked changes, text boxes, or appendices occur in the supplied package. Footnote/endnote parts contain only separator placeholders.
- Source paragraph text and table-cell text were compared with the converted source blocks. The nine-line C++ example was checked character-for-character. Trademarks, copyright signs, source spelling, repeated statements and conflicting labels are retained.
- The original PNG bytes are embedded unchanged. All images were visually inspected; figure-only specifications, warnings, screenshot settings and readable labels are transcribed. Very small or obscured photographic markings, the undecoded machine-readable code and exact graphical geometry cannot be completely conveyed by a textual description. The retained originals preserve those pixels.
- The Word footer contains PAGE and NUMPAGES fields with saved results `1 / 1`; those saved results are preserved, not asserted to be a verified page count. The metadata page count is also 1. A Word page render could not be produced because native Word automation returned access denied and the bundled runtime supplies no LibreOffice renderer on Windows. Thus original page pagination, repeated header/footer placement, clipping, and page-by-page layout could not be independently verified. The full underlying document content was still inspected.
- The source applies a Heading 2 style to the whole Description paragraph. That style is represented as a Markdown subheading without rewriting the paragraph. Empty spacer paragraphs and Word-specific typography are not reproduced as fixed page layout.
- The AI companion explicitly records source conflicts: Figure 2 power/module/current labels versus body guidance, Figure 3 hole radius versus Section 5.2/Figure 7 diameter, screenshot upload speed versus the qualified UART recommendation, and missing references/revision history. The source's original statements remain unchanged here.
