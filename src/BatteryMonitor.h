//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _BATTERY_MONITOR_H_
#define _BATTERY_MONITOR_H_

#include "GeekShieldConfig.h"

class BatteryMonitor {
  public:
    enum class EventType {BatteryWarning, BatteryLow};

    static BatteryMonitor* instance() {
      static BatteryMonitor inst;
      return &inst;
    }

    void setup();
    void task();

  private:
    BatteryMonitor() {};
    
    const GeekShieldConfig *config;

    double readBatteryVoltage();
    double calibrateVoltage(double value);
};

#endif