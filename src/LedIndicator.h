//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _LED_INDICATOR_H_
#define _LED_INDICATOR_H_

#include <Arduino.h>

#include <BluepadHub.h>
#include <ESP32PWM.h>    

class LedIndicator : public bluepadhub::StatusIndicator {
  public:

    LedIndicator() {};

    virtual void begin(int16_t pin, bool setBluepadHubStatusIndicator = true);
    virtual void clear();

    virtual void showStatusPattern();
    virtual void showEventPattern();

    void setBrightness(uint8_t _ledBrightness);

  private:
    void setLedLevel(double level);

    uint8_t ledBrightness = 255;

    ESP32PWM pwm;
};  

#endif