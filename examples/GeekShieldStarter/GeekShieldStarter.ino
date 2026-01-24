//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <GeekShieldStarter.h>

GeekShieldStarter<GeekShieldV2> shield;

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
