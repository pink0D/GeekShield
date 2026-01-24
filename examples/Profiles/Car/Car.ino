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

Profile:  Car with GeekServo for steering and Power Functions motors for moving

Controls: 
1. Left stick to steer
2. Right trigger to accelerate, hold triangle to reverse
3. Left trigger to brake

Easy mode (press L1 + R1 to activate this mode):
1. Left stick up/down to move forward/reverse
2. Right stick to steer

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

    // specify max servo rotation angle in degrees so it does not exceed what the CV joints can handle
    // setServoMaxAngle(SERVO_1, 60);

    // PWM duty cycle for motors can be limited from 0 to 100% (default is full range)
    // usually it is recommended to set minimum 25% for Power Function motors
    setMotorLimits(MOTOR_A, 0.25, 1.0);
    setMotorLimits(MOTOR_B, 0.25, 1.0);
  };

  void processBluepadController(BluepadController* ctl) {
    
    double speed = normalizeTriggerInput(ctl->throttle());  // R trigger to accelerate
    double steer = normalizeStickInput(ctl->axisX());       // L stick to steer

    bool brake =  normalizeTriggerInput(ctl->brake()) > 0;  // L trigger to brake
    //brake = false; // disable braking

    // reverse while holding Y or R1
    if (ctl->y() || ctl->r1()) { 
      speed = -speed;
    }

    // change to 'true' to make easy mode default
    static bool easyMode = false;   

    // toggle easy mode by pressing L1 + R1
    if (wasClicked(ctl->l1() && ctl->r1()))
      easyMode =! easyMode;

    if (easyMode) {
      speed = -normalizeStickInput(ctl->axisY()); // move forward when stick is UP
      steer = normalizeStickInput(ctl->axisRX()); 
    }

    if (brake) {
      brakeMotor(MOTOR_A);
      brakeMotor(MOTOR_B);
    } else {
      updateMotor(MOTOR_A, speed); // change to "-speed" to change motor A direction
      updateMotor(MOTOR_B, speed); // change to "-speed" to change motor B direction
    }

    updateServo(SERVO_1, steer); // change to "-steer" to change steering direction
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
