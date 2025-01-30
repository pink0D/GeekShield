# GeekShield
GeekShield is a DIY shiled for ESP32 with Power Functions and GeekServo motors support

[![GeekShield board](https://i.postimg.cc/nsvQcyZ2/IMG-9978-thumb.jpg)](https://postimg.cc/nsvQcyZ2)

[More images](https://postimg.cc/gallery/sQHv5dx)

# Features
- Classic bluetooth gamepad support (Dualshock 4, DualSense, Xbox Wireless Controller or any other)
- Direct bluetooth connection (no iOS/Android apps needed)
- 2x Power Functions ports, maximum 1.5A at peak per channel (2x Buggy Motors can be used together)
- 5x classic servo (3-wire) connectors (only 1 GeekServo can be controlled when using with PF motors). 
- Pluggable 2S LiPo battery
- Battery discharge protection
- Ready to use Arduino Library for easy programming with Arduino IDE

# Hardware

Materials needed to make the GeekShield:
1. The PCB
  - Files for PCB production are located in [extras](https://github.com/pink0D/GeekShield/tree/main/extras) directory of this repo.
  - However, there are very few components needed so you can make the GeekShield even on a [breadboard](https://postimg.cc/V5RcGDs9)
2. ESP32-CAM board
3. ESP32-CAM-MB or any USB-Serial adapter for uploading Arduino code
4. [Pololu Mini Pushbutton Power Switch](https://www.pololu.com/product/2808)
5. [Pololu DRV8835 Dual Motor Driver Carrier](https://www.pololu.com/product/2135)
6. 2S LiPo battery with high discharge rate
7. Battery connector
8. Power Functions extension cable which will be glued on the PCB
9. 3x6 mm clickable button (through-the-hole/THT mount)
10. Resistors:
-  1x 2.2kOhm
-  1x 4.7kOhm
11. Pin headers
- 2x female 8-pin
- 4x female 7-pin
- 3x male 5-pin headers (black, yellow and red to distinguish servo pins)
- 1x angled female 3-pin 
12. Other basic soldeting accessories

# Software

1. Add [ESP32](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) and [Bluepad32](https://bluepad32.readthedocs.io/en/latest/plat_arduino/) boards in Arduino IDE using the Board Manager:
- https://espressif.github.io/arduino-esp32/package_esp32_index.json
- https://raw.githubusercontent.com/ricardoquesada/esp32-arduino-lib-builder/master/bluepad32_files/package_esp32_bluepad32_index.json
2. Install [ESP32Servo](https://github.com/madhephaestus/ESP32Servo) library using the Library Manager
3. Manually add GeekShield library to your workspace
- create **GeekShield** directory inside *workspace/librarires*
- copy all contents of this repo to *workspace/librarires/GeekShield*
- restart Arduino IDE
4. Review the examples by opening *File->Examples->GeekSheild->*
- TestBluepad32 is good to start with to ensure that you've installed Bluepad32 correctly and your gamepad can connect to the board
5. Attach either [ESP32-CAM-MB](https://randomnerdtutorials.com/upload-code-esp32-cam-mb-usb/) or [USB-Serial adapter](https://randomnerdtutorials.com/program-upload-code-esp32-cam/)
6. Set sketch upload parameters:
- Board: *esp32_bluepad32->ESP32 Dev Module*
- PSRAM: **disabled**
- Leave defaults for other parameters
7. Before uploading the code, update the **config.batteryAdcScale** value inside the *setup()* function
- set this value to *(R1+R2) / R1* according to the values of resistors you've soldered to the board.
- *batteryAdcScale* usually should have a value around 2.8-3.1
8. Finally, upload the sketch and try the GeekShield
