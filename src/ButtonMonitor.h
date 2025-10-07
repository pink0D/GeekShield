//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _BUTTON_MONITOR_H_
#define _BUTTON_MONITOR_H_

#include "GeekShield.h"

class ButtonMonitor {
  public:
    enum class ClickType {Short, Long, Hold};
    enum class EventType {Press, Release};

    static ButtonMonitor* instance() {
      static ButtonMonitor inst;
      return &inst;
    }

    void setup();
    void task();

  private:
    ButtonMonitor() {};

    const GeekShieldConfig *config;

    typedef int (ButtonMonitor::*readPinFuncPtr_t)(); 
    readPinFuncPtr_t readPinFuncPtr;

    int readDigitalPin();
    int readAnalogPin();
};

#endif