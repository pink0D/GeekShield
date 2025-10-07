//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _PFMotor_h
#define _PFMotor_h

#include <ESP32PWM.h>

#include "TechnicMotor.h"

class PFMotor : public TechnicMotor {
public:
  friend class GeekShield;

  enum class PwmType {Exact, Scaled, Fixed3, Fixed15};

  void setPwmType(PwmType pwm_type) {
    this->pwm_type = pwm_type;
  };
  
  void setPwmMinimumDuty(double min_duty) {
    this->config_min_duty = min_duty;
  };

protected:
  TechnicMotor* setup(int pin_c1, int pin_c2, PwmType pwm_type = PwmType::Exact);

  void release();

  // c1 and c2 must be within 0..1 range
  virtual void updateOutput(double new_c1, double new_c2, bool brake = false);

private:
  PwmType pwm_type = PwmType::Exact;
  ESP32PWM pwm_c1;
  ESP32PWM pwm_c2;

  double config_min_duty;

  void output_pwm(double c1, double c2, bool brake);

  double convert_pwm_3(double c);
  double convert_pwm_15(double c);
  double convert_scaled(double c);
};

#endif
