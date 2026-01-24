//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _ANALOG_BUTTON_H_
#define _ANALOG_BUTTON_H_

#include <Arduino.h>

#include <MultiFunctionButton.h>

class AnalogButton : public bluepadhub::MultiFunctionButton {
  public:
    AnalogButton() {};

  protected: 
    virtual int readButtonState(int pin) {

        if (analogReadMilliVolts(pin) > 500)
            return HIGH;

        return LOW;
    };

};

#endif