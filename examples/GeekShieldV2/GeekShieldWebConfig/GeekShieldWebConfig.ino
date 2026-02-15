//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <GeekShield.h>
#include <web/UniProfile.h>

bluepadhub::UniProfile<GeekShieldV2> uni;

//  Arduino setup function
void setup() {
  // uncomment delay to see full Serial output during board startup in Arduino IDE
  //delay(2000) 
  uni.setDefaultDeviceName("GeekShield");
  uni.setDefaultMinPWM(25);
  uni.begin();
}

// Arduino loop function
void loop() {
  uni.update();
  //no delay here because it's inside uni.update()
}
