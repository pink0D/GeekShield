//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _CONTROL_PROFILE_H_
#define _CONTROL_PROFILE_H_

class GeekShield;
class GeekShieldConfig;

class ControlProfile {
  public:
    ControlProfile();

    virtual void setup(GeekShield *shield) {};
    virtual void processController(ControllerPtr ctl) {};

    static bool checkButtonClick(bool &currentValue, bool newValue) {
      bool click = false;
      if (newValue) {
        click = !currentValue;
      }
      currentValue = newValue;
      return click;
    }

    // returns normalized value in 0..1 range, with deadzones correction
    double normalizeTriggerInput(int input);

    // returns normalized value in -1..1 range, with center and outer deadzone correction
    double normalizeStickInput(int input);

    void initSettings(const char* nvs_namespace);

    int getSettingInt(const char* nvs_key, int default_value = 0);
    void updateSettingInt(const char* nvs_key, int new_value);

    double getSettingDouble(const char* nvs_key, double default_value = 0);
    void updateSettingDouble(const char* nvs_key, double new_value);

  private:
    const GeekShieldConfig *config;
    const char* nvs_namespace;
};

#endif