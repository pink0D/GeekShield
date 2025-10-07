//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _RGB_INDICATOR_H_
#define _RGB_INDICATOR_H_

#include "LedIndicator.h"

#include <Adafruit_NeoPixel.h>

class RgbIndicator : public LedIndicator {
  public:

    static RgbIndicator* instance() {
      static RgbIndicator inst;
      return &inst;
    };

    virtual void setup();

    virtual void showStatusPattern();
    virtual void showEventPattern();

  private:
    RgbIndicator() {};

    const GeekShieldConfig *config;

    Adafruit_NeoPixel *pixels;

    void setLedColor(unsigned long color);
};

#endif