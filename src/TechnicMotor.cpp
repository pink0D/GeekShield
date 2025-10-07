//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "TechnicMotor.h"
#include "GeekShield.h"

TechnicMotor* TechnicMotor::setup() {

  setAntiJitter(GeekShield::instance()->getConfig()->motorAntiJitterDefault);
  invertControl(false);
  limitControl(1.0);

  return this;
}

void TechnicMotor::updateMotor(double new_ctl_val, bool brake) {

    if (new_ctl_val < -ctl_limit)
      new_ctl_val = -ctl_limit;

    if (new_ctl_val > ctl_limit)
      new_ctl_val = ctl_limit;
    
    double delta = new_ctl_val - ctl_val;
    // update motor outputs if new control value differs from curent +/- delta 
    if (abs(delta) >= min_delta)
      ctl_val = new_ctl_val;
    
    // set to zero if new value is near 0.0
    if (abs(new_ctl_val) < min_delta)
      ctl_val = 0;

    // set to ctl_limit if new value is near limit
    if (new_ctl_val < -ctl_limit+min_delta)
      new_ctl_val = -ctl_limit;
    if (new_ctl_val > ctl_limit-min_delta)
      new_ctl_val = ctl_limit;

    double inverted_value = invert_ctl ? -ctl_val : ctl_val;

    if (inverted_value > 0) 
      updateOutput(inverted_value,0,brake);
    else 
      updateOutput(0,abs(inverted_value),brake);
  };
