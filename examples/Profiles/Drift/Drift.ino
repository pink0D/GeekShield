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

Profile:  Drift car with GeekServo for steering and independent motors for left and right rear wheels

Controls: left stick to steer, right trigger to accelerate, hold Y button for reverse

Additionally:
1) Left trigger when steering - decrease motor torque for inner wheel while making a turn
2) Left trigger when not steering - motor braking to stop the car quicly 
3) Right stick - Drift mode, motors rotate in opposite directions

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
    
    // get inputs from controller
    double accelerator = normalizeTriggerInput(ctl->throttle());
    double steer = normalizeStickInput(ctl->axisX());
    double brake = normalizeTriggerInput(ctl->brake());
    double drift = normalizeStickInput(ctl->axisRX());
    bool stop = false;

    double k_A = 1.0;
    double k_B = 1.0;

    if (steer < 0) {      // left turn
      k_A = 1.0 - brake;  // decrease left motor torque
    }
    else if (steer > 0) { // right turn
      k_B = 1.0 - brake;  // decrease right motor torque
    } 
    else if (brake > 0) { // full brake both motors quickly
      stop = true;
    }

    // reverse
    if (ctl->y()) {
      accelerator = -accelerator;
    }

    // drift mode
    if (abs(drift) > 0) {

        accelerator = abs(drift);

        if (drift < 0) { // left rotation
            k_A = -1.0;
            k_B = 1.0;
        }
        if (drift > 0) { // right rotation
            k_A = 1.0;
            k_B = -1.0;
        }
    }
  
    if (stop) {
      brakeMotor(MOTOR_A);
      brakeMotor(MOTOR_B);
    } else {
      updateMotor(MOTOR_A, accelerator*k_A); // change to "-accelerator*k_A" to change motor A direction
      updateMotor(MOTOR_B, accelerator*k_B); // change to "-accelerator*k_B" to change motor B direction
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
