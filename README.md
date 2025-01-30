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
3. ESP32-CAM board
4. ESP32-CAM-MB or any USB-Serial adapter for uploading Arduino code
5. [Pololu Mini Pushbutton Power Switch](https://www.pololu.com/product/2808)
6. [Pololu DRV8835 Dual Motor Driver Carrier](https://www.pololu.com/product/2135)
7. 2S LiPo battery with high discharge rate
8. Battery connector
9. Power Functions extension cable which will be glued on the PCB
10. 3x6 mm clickable button (through-the-hole/THT mount)
11. Pin headers
- 2x female 8-pin
- 4x female 7-pin
- 3x male 5-pin headers (black, yellow and red to distinguish servo pins)
- 1x angled female 3-pin 
12. Other basic soldeting accessories
