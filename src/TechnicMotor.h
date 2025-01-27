//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _TechnicMotor_h
#define _TechnicMotor_h

class TechnicMotor {
public:
  void updateMotor(double new_ctl_val, bool brake = false);

  double getControlValue() {
    return ctl_val;
  };

  void setAntiJitter(double min_delta) {
    this->min_delta = min_delta;
  }

  double getAntiJitter() {
    return min_delta;
  }

  void invertControl(bool invert_ctl) {
    this->invert_ctl = invert_ctl;
  };

  void limitControl(double ctl_limit) {
    this->ctl_limit = ctl_limit;
  }

protected:
  // c1 and c2 must be within 0..1 range
  virtual void updateOutput(double new_c1, double new_c2, bool brake = false) {};
  void setup();
  virtual void release();

private:
  double ctl_val = 0;
  double min_delta = 0;
  double ctl_limit = 1.0;
  bool invert_ctl = false;
};

#endif
