//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "GeekServo.h"

TechnicMotor* GeekServo::setup(int pin, int servo_min, int servo_max){
  this->servo_min = servo_min;
  this->servo_max = servo_max;

  attach(pin,servo_min,servo_max);

  return TechnicMotor::setup();
}

void GeekServo::release() {

  if (attached()) {
    write(0);
    detach();
  }
}

// output control signals to servo
void GeekServo::updateOutput(double new_c1, double new_c2, bool brake) {

  // check that the motor has been initialized
  if (!attached())
    return; // don't update any outputs if it was not initialized

  int st = (servo_max - servo_min) / 2;
  int us = (servo_min + servo_max) / 2 - st*new_c1 + st*new_c2;
  writeMicroseconds(us);
}
