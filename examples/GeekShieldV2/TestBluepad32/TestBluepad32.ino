//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <GeekShield.h>

class : public GeekShieldV2 {

  void setup() {

    enablePairingAfterStartup = true;

    Serial.println("Waiting for controller connection");
  };

  void processBluepadController(BluepadController* ctl) {

    Serial.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, L1:%d, R1:%d, "
        "misc: 0x%02x, A:%d, B:%d, X:%d, Y:%d\n",
        ctl->index(),        // Controller Index
        ctl->dpad(),         // D-pad
        ctl->buttons(),      // bitmask of pressed buttons
        ctl->axisX(),        // (-511 - 512) left X Axis
        ctl->axisY(),        // (-511 - 512) left Y axis
        ctl->axisRX(),       // (-511 - 512) right X axis
        ctl->axisRY(),       // (-511 - 512) right Y axis
        ctl->brake(),        // (0 - 1023): brake button
        ctl->throttle(),     // (0 - 1023): throttle (AKA gas) button
        ctl->l1(),
        ctl->r1(),
        ctl->miscButtons(),  // bitmask of pressed "misc" buttons
        ctl->a(),
        ctl->b(),
        ctl->x(),
        ctl->y()
    );

  };

} shield;


//  Arduino setup function
void setup() {
  // uncomment delay to see full Serial output during board startup in Arduino IDE
  //delay(2000) 
  shield.begin();
}

// Arduino loop function
void loop() {
  shield.update();
  //no delay here because it's inside shield.update()
}
