//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <GeekShield.h>

/*

Profile:  Tank

Controls: left & right sticks drive tracks independently

*/

// change to GeekShieldDIY<1,2> if that board is used
class : public GeekShieldV2 { 

  // uncomment for GeekShieldDIY
  /*
  double batteryAdcScale() {
  
    return 3.5; // modify this value so battery voltage measured by ESP32 matches the value on multimeter
  };
  */

  void setup() {

    // automatically start pairing every time the controller is started
    // by default, pairing mode is activated by holding button for 5 secs or 10secs to forget pairing

    enablePairingAfterStartup = true;

    // PWM duty cycle for motors can be limited from 0 to 100% (default is full range)
    // usually it is recommended to set minimum 25% for Power Function motors
    setMotorLimits(MOTOR_A, 0.25, 1.0);
    setMotorLimits(MOTOR_B, 0.25, 1.0);
  };

  void processBluepadController(BluepadController* ctl) {

    double drive_A = normalizeStickInput(ctl->axisY());
    double drive_B = normalizeStickInput(ctl->axisRY());
    
    updateMotor(MOTOR_A, drive_A); // change to "-drive_A" to change motor A direction
    updateMotor(MOTOR_B, drive_B); // change to "-drive_B" to change motor B direction
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
