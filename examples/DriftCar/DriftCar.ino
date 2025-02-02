// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <GeekShield.h>

/*
Drift car driving

Controls - left stick to steer, right trigger to accelerate, hold Y button for reverse

Additionally:
1) Left trigger when steering - decrease motor torque for inner wheel while making a turn
2) Left trigger when not steering - motor braking to stop the car quicly 
3) Right stick - Drift mode - motors rotate in opposite directions

*/

class CarDriftProfile : public ControlProfile {
  private:
    PFMotor *motorA;
    PFMotor *motorB;
    GeekServo *geekServo;

  public:
    void setup(GeekShield *shield) {
      motorA = shield->getPFMotor(GeekShield::Port::MotorA); motorA->invertControl(true);      
      motorB = shield->getPFMotor(GeekShield::Port::MotorB);

      geekServo = shield->getGeekServo(GeekShield::Port::Servo1);
      geekServo->invertControl(true); // invert direction if links are in front of the wheels
      //geekServo->limitControl(0.75);  // limit maximum rotation 
    };

    void processController(ControllerPtr ctl) {  

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

      if (ctl->y()) // reverse
        accelerator = -accelerator;

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

      motorA->updateMotor(accelerator*k_A, stop);
      motorB->updateMotor(accelerator*k_B, stop);
      geekServo->updateMotor(steer);
    };
};

/*
Main sketch code
*/

GeekShield *shield = GeekShield::instance();

void setup() {

  GeekShieldConfig config;
  config.batteryAdcScale = 3.5; // voltage divider factor

  shield->setup(config);

  shield->addProfile(new CarDriftProfile());
}

void loop() {
  shield->loop();
}
