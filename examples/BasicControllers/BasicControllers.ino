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
Profile:  Car with Power Functions servo and driving motors

Controls: left stick to steer, right trigger to accelerate, hold triangle to reverse
*/

class CarProfile : public ControlProfile {
  private:
    PFMotor *motor_drive;
    PFMotor *motor_steer;

  public:
    void setup(GeekShield *shield) {
      motor_drive = shield->getPFMotor(GeekShield::Port::MotorA); 
      motor_steer = shield->getSimpleServo(GeekShield::Port::MotorB); // simple servo: -90/0/+90 degrees
      // alternatevely, a proportional PF servo can be configured
      //motor_steer = shield->getProportionalServo(GeekShield::Port::MotorB); 
    };

    void processController(ControllerPtr ctl) {  

      // get inputs from controller
      double accelerator = normalizeTriggerInput(ctl->throttle());
      double steer = normalizeStickInput(ctl->axisX());

      if (ctl->y()) // reverse
        accelerator = -accelerator;

      motor_drive->updateMotor(accelerator);
      motor_steer->updateMotor(steer);
    };
};

/*
Profile:  Tracked vehicle

Controls: left & right sticks drive tracks independently
*/

class TrackedProfile : public ControlProfile {
  private:
    PFMotor *motorA;
    PFMotor *motorB;

  public:
    void setup(GeekShield *shield) {
      motorA = shield->getPFMotor(GeekShield::Port::MotorA); 
      motorB = shield->getPFMotor(GeekShield::Port::MotorB);

      // tracked vehicles usually have gears on different sides of the motors
      // so one of the motors needs to be inverted to move forward 
      motorA->invertControl(true); 
    };

    void processController(ControllerPtr ctl) {  

      // get inputs from controller
      double drive_A = normalizeStickInput(ctl->axisY());
      double drive_B = normalizeStickInput(ctl->axisRY());

      motorA->updateMotor(drive_A);
      motorB->updateMotor(drive_B);
    };
};

/*
Profile:  Car with GeekServo for steering

Controls: left stick to steer, right trigger to accelerate, hold triangle to reverse
*/

class GeekProfile : public ControlProfile {
  private:
    PFMotor *motorA;
    PFMotor *motorB;
    GeekServo *geekServo;

    // limit maximum servo rotation to 67.5 degrees (75% of full 90 degree turn)
    double steerLimit = 0.75;

  public:
    void setup(GeekShield *shield) {
      motorA = shield->getPFMotor(GeekShield::Port::MotorA); 
      motorB = shield->getPFMotor(GeekShield::Port::MotorB);

      geekServo = shield->getGeekServo(GeekShield::Port::Servo1);
      geekServo->invertControl(true);       // invert direction if links are in front of the wheels
      geekServo->limitControl(steerLimit);  // set rotation limit
    };

    void processController(ControllerPtr ctl) {  

      // get inputs from controller
      double accelerator = normalizeTriggerInput(ctl->throttle());
      double steer = normalizeStickInput(ctl->axisX());

      if (ctl->y()) // reverse
        accelerator = -accelerator;

      // update both PF motors, it's ok if only one is attached, but using two motors on the same shaft produces 2x torque
      motorA->updateMotor(accelerator);
      motorB->updateMotor(accelerator);

      // adjust input with defined limit, so there's no large deadzone when input is higher than the limit
      geekServo->updateMotor(steer * steerLimit); 
    };
};

/*
Callback for profile switching 
*/

void profileSwitcher(ControllerPtr ctl) {

  static bool btnA = false;
  if (ControlProfile::checkButtonClick(btnA, ctl->a())) 
    GeekShield::instance()->switchActiveProfile();
}

/*
Main sketch code
*/

GeekShield *shield = GeekShield::instance();

void setup() {

  GeekShieldConfig config;
  config.batteryAdcScale = 3.5; // voltage divider factor

  shield->setup(config);

  shield->addProfile(new CarProfile());
  shield->addProfile(new TrackedProfile());
  shield->addProfile(new GeekProfile());

  shield->registerControllerCallback(profileSwitcher);
}

void loop() {
  shield->loop();
}
