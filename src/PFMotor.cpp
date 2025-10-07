//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <Arduino.h>
#include "PFMotor.h"
#include "GeekShield.h"

TechnicMotor* PFMotor::setup(int pin_c1, int pin_c2, PwmType pwm_type) {

  const GeekShieldConfig *config = GeekShield::instance()->getConfig();

  pwm_c1.attachPin(pin_c1, config->pfMotorPwmFrequency);
  pwm_c2.attachPin(pin_c2, config->pfMotorPwmFrequency);

  setPwmType(pwm_type);
  config_min_duty = config->pfMotorPwmMinimumDuty;
  
  return TechnicMotor::setup();
}

void PFMotor::release() {

  if (pwm_c1.attached()) {
    pwm_c1.write(0);
    pwm_c1.detachPin(pwm_c1.getPin());
  }

  if (pwm_c2.attached()) {
    pwm_c2.write(0);
    pwm_c2.detachPin(pwm_c2.getPin());
  }
}

void PFMotor::output_pwm(double c1, double c2, bool brake) {

  // check that the motor has been initialized
  if ( (!pwm_c1.attached()) || (!pwm_c2.attached()) )
    return; // don't update any outputs if it was not initialized
  
  if (brake) {
    pwm_c1.writeScaled(1.0);
    pwm_c2.writeScaled(1.0);
    return;
  }

  if (c1 >= config_min_duty)
    pwm_c1.writeScaled(c1);
  else
    pwm_c1.write(0);

  if (c2 >= config_min_duty)
    pwm_c2.writeScaled(c2);
  else
    pwm_c2.write(0);
}

// update signal for motor
// c1 and c2 must be within 0..1 range
void PFMotor::updateOutput(double new_c1, double new_c2, bool brake) {

  if (new_c1 < 0) 
    new_c1 = 0;

  if (new_c2 < 0) 
    new_c2 = 0;

  if (new_c1 > 1) 
    new_c1 = 1;

  if (new_c2 > 1) 
    new_c2 = 1;

  if (pwm_type == PwmType::Fixed3) {
    new_c1 = convert_pwm_3(new_c1);
    new_c2 = convert_pwm_3(new_c2);
  }

  if (pwm_type == PwmType::Fixed15) {
    new_c1 = convert_pwm_15(new_c1);
    new_c2 = convert_pwm_15(new_c2);
  }

  if (pwm_type == PwmType::Scaled) {
    new_c1 = convert_scaled(new_c1);
    new_c2 = convert_scaled(new_c2);
  }

  output_pwm(new_c1, new_c2, brake);
}

double PFMotor::convert_pwm_3(double c){
  if (c > 0.25)
    return 1;

  return 0;
}

double PFMotor::convert_pwm_15(double c){
  // returned value * 255 should match fixed PWM byte values 0-66-97-128-162-194-225-255

  if (c > 0.88)
    return 1;

  if (c > 0.75)
    return 0.885;

  if (c > 0.62)
    return 0.762;

  if (c > 0.49)
    return 0.637;

  if (c > 0.36)
    return 0.505;

  if (c > 0.23)
    return 0.381;

  if (c > 0.10)
    return 0.261;

  return 0;
}

double PFMotor::convert_scaled(double c) {
  if (c < getAntiJitter())
    return 0;

  return config_min_duty + (1.0 - config_min_duty) * c;
}