//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _GEEKSHIELD_H_
#define _GEEKSHIELD_H_

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

#include <Bluepad32.h>

#include "extras.h"

#include "GeekShieldConfig.h"
#include "ControlProfile.h"
#include "TechnicMotor.h"
#include "PFMotor.h"
#include "GeekServo.h"
#include "LedIndicator.h"

typedef std::function<void(ControllerPtr controller)> ControllerCallback;

class GeekShield {
  public:
    static GeekShield* instance() {
      static GeekShield inst;
      return &inst;
    }

    const GeekShieldConfig* getConfig() {
      return &config;
    }

    void setup(GeekShieldConfig config) {
        this->config = config;
        setup();
    };

    void registerControllerCallback(const ControllerCallback &callback) {
      this->controllerCallback = callback;
    }

    void loop();

    void addProfile(ControlProfile *profile);
    void switchActiveProfile();

    enum class Port {MotorA, MotorB, Servo1, Servo2, Servo3, Servo4, Servo5};

    PFMotor* getPFMotor(Port port, PFMotor::PwmType pwmType = PFMotor::PwmType::Scaled);
    GeekServo* getGeekServo(Port port, int servoMin = 1000, int servoMax = 2000);
    
    PFMotor* getProportionalServo(Port port) {  // original 15-positional servo
      return getPFMotor(port, PFMotor::PwmType::Fixed15);
    };
    PFMotor* getSimpleServo(Port port) {   // simplified 3-positional servo
      return getPFMotor(port, PFMotor::PwmType::Fixed3);
    };

  protected:

    friend class LedIndicator;
    friend class BatteryMonitor;
    friend class ButtonMonitor;

    void setup();

    int getActiveProfileDisplayIndex() {
      return activeProfile + 1;
    };

    void enablePairing();
    void resetPairing();

    void lowBatteryWarning();
    void powerOff();
    

  private:
    GeekShield();

    GeekShieldConfig config;

    void init_nvs();

    void disconnectController();

    static void onConnectedController(ControllerPtr ctl);
    static void onDisconnectedController(ControllerPtr ctl);

    ControllerPtr bp32Controller = nullptr;

    long time_auto_power_off    = 0;
    long time_controller_update = 0;

    bool lowPowerMode = false;

    ControlProfile* profiles[MAX_PROFILES];
    int activeProfile = 0;
    int numProfiles = 0;

    ControllerCallback controllerCallback = nullptr;

    // TODO: add dynamic motor creation upon request by ControlProfile
    PFMotor motorA;
    PFMotor motorB;
    GeekServo motorServo[MAX_GEEKSERVOS];

    void stopMotors();
    void releaseMotors();

    bool checkIdleMotors();
};

#endif