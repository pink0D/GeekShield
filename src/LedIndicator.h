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

#include "GeekShield.h"

class LedIndicator {
  public:
    enum class StatusPattern {None, Idle, Pairing, Connected, BatteryLow, Settings};
    enum class EventPattern {None, Reset, ProfileSelect, SettingSaved};

    static LedIndicator* instance() {
      static LedIndicator inst;
      return &inst;
    };

    void setup();
    void task();

    void setStatusPattern(StatusPattern statusPattern) {
      this->statusPattern = statusPattern;
    };

    void setEventPattern(EventPattern eventPattern) {
      this->eventPattern = eventPattern;
    };

  private:
    LedIndicator() {};

    const GeekShieldConfig *config;

    void setLedLevel(uint8_t val);
    void patternDelayMillis(int timeout);

    StatusPattern statusPattern = StatusPattern::None;
    EventPattern eventPattern = EventPattern::None;

    ESP32PWM pwm;
};

#endif