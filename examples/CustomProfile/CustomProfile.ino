// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <GeekShield.h>

/*
Car controls with customizeable steering limit:
1) driving mode: left stick to steer, right trigger to accelerate, hold triangle to reverse
2) settings mode: to enter this mode, press X button
   when in settings mode, use left stick to set maximum steering position, then save it by pressing X again
*/

class CarCustomProfile : public ControlProfile {
  private:
    PFMotor *motorA;
    PFMotor *motorB;
    GeekServo *geekServo;

    bool settingsMode = false;
    LedIndicator *led;

    double steering_limit;
    const char* key_steering_limit = "steering_limit";

  public:
    void setup(GeekShield *shield) {
      motorA = shield->getPFMotor(GeekShield::Port::MotorA); 
      motorB = shield->getPFMotor(GeekShield::Port::MotorB);
      geekServo = shield->getGeekServo(GeekShield::Port::Servo1);

      led = LedIndicator::instance();

      // read settings from flash
      initSettings("car_settings");
      steering_limit = getSettingDouble(key_steering_limit, 1.0); // default to 1.0 if no value was previously saved
    };

    void processController(ControllerPtr ctl) {  

      // get inputs from controller
      double accelerator = normalizeTriggerInput(ctl->throttle());
      double steer = normalizeStickInput(ctl->axisX());

      static bool btnX = false;
      // ENTER/EXIT settings mode
      if (checkButtonClick(btnX, ctl->x())) {
        
        if (settingsMode == false) { // enter settings mode
          settingsMode = true;
        } else { // exit settings mode & save settings
          settingsMode = false;

          if (abs(steer) > 0) { // don't save zero limit
          
            steering_limit = abs(steer); // set the limit to absolute value of current input
            updateSettingDouble(key_steering_limit, steering_limit); // save setting to flash
  
            led->setEventPattern(LedIndicator::EventPattern::SettingSaved);
          }
        }
      }

      if (settingsMode) {
        led->setStatusPattern(LedIndicator::StatusPattern::Settings);

        // in settings mode, servo is allowed to turn within the full range from input
        geekServo->updateMotor(steer); 

        // no motor output in settings mode
        motorA->updateMotor(0);
        motorB->updateMotor(0);

      } else { // driving mode

        led->setStatusPattern(LedIndicator::StatusPattern::Connected);

        // limit input up to saved limit
        geekServo->updateMotor(steer*steering_limit); 
 
        if (ctl->y()) // reverse
          accelerator = -accelerator;

        // update other motors as usual
        motorA->updateMotor(accelerator);
        motorB->updateMotor(accelerator);
      }
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

  shield->addProfile(new CarCustomProfile());
}

void loop() {
  shield->loop();
}
