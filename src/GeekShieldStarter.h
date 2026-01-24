//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _GEEKSHIELD_STARTER_H_
#define _GEEKSHIELD_STARTER_H_

#include "GeekShield.h"
#include "WebConfig.h"
#include "GeekShieldConfig.h"


template<class BaseClass>
class GeekShieldStarter : public BaseClass, private bluepadhub::WebConfig {

    public:
        void begin() {
            BaseClass::begin();
        };

    private:
        GeekShieldConfig config;

        void setup() {

            //this->enablePairingAfterStartup = true;

            this->indicator.setBrightness(20);

            // uncomment to adjust controller sensivity
            //
            // controllerStickDeadzoneLow = 50;        // 0 = lowest value for stick input
            // controllerStickDeadzoneHigh = 500;      // 512 = highest value for stick input
            // controllerTriggerDeadzoneLow = 5;       // 0 = lowest value for trigger input
            // controllerTriggerDeadzoneHigh = 1000;   // 1024 = highest value for trigger input 

            WebConfig::begin();
        };

        void processBluepadController(BluepadController* ctl) {

            // Options button
            if (this->wasClicked(ctl->miscButtons() & 0x04))
                setDiscoverable(60);

            for (int i=0; i<this->getNumMotors(); i++) {

                double input = getInputValue(ctl, config.pf[i].input);
                double brake = getInputValue(ctl, config.pf[i].brake);

                if (config.pf[i].invertInput)
                    input = -input;

                if (getButtonValue(ctl, config.pf[i].invertButton))
                    input = -input;

                if (brake)
                    this->brakeMotor(i);
                else
                    this->updateMotor(i, input);
            }

            for (int i=0; i<this->getNumServos(); i++) {

                double input = getInputValue(ctl, config.servo[i].input);

                if (config.servo[i].invertInput)
                    input = -input;

                if (getButtonValue(ctl, config.servo[i].invertButton))
                    input = -input;

                this->updateServo(i, input);
            }
        };

        double getInputValue(BluepadController* ctl, GeekShieldConfig::ControllerInput input) {

            if (input == GeekShieldConfig::ControllerInput::NullInput)
                return 0;

            if (input == GeekShieldConfig::ControllerInput::L2)
                return this->normalizeTriggerInput(ctl->brake());

            if (input == GeekShieldConfig::ControllerInput::R2)
                return this->normalizeTriggerInput(ctl->throttle());

            if (input == GeekShieldConfig::ControllerInput::LeftStickX)
                return this->normalizeStickInput(ctl->axisX());

            if (input == GeekShieldConfig::ControllerInput::LeftStickY)
                return this->normalizeStickInput(ctl->axisY());

            if (input == GeekShieldConfig::ControllerInput::RightStickX)
                return this->normalizeStickInput(ctl->axisRX());

            if (input == GeekShieldConfig::ControllerInput::RightStickY)
                return this->normalizeStickInput(ctl->axisRY());

            return 0;
        }

        bool getButtonValue(BluepadController* ctl, GeekShieldConfig::ControllerButton button) {

            if (button == GeekShieldConfig::ControllerButton::NullButton)
                return false;

            if (button == GeekShieldConfig::ControllerButton::L1)
                return ctl->l1();

            if (button == GeekShieldConfig::ControllerButton::R1)
                return ctl->r1();

            if (button == GeekShieldConfig::ControllerButton::L3)
                return ctl->thumbL();

            if (button == GeekShieldConfig::ControllerButton::R3)
                return ctl->thumbR();

            if (button == GeekShieldConfig::ControllerButton::Square)
                return ctl->x();

            if (button == GeekShieldConfig::ControllerButton::Triangle)
                return ctl->y();

            if (button == GeekShieldConfig::ControllerButton::Cross)
                return ctl->a();

            if (button == GeekShieldConfig::ControllerButton::Circle)
                return ctl->b();

            if (button == GeekShieldConfig::ControllerButton::DpadUp)
                return ctl->dpad() & 0x01;

            if (button == GeekShieldConfig::ControllerButton::DpadDown)
                return ctl->dpad() & 0x02;

            if (button == GeekShieldConfig::ControllerButton::DpadLeft)
                return ctl->dpad() & 0x04;

            if (button == GeekShieldConfig::ControllerButton::DpadRight)
                return ctl->dpad() & 0x08;

            return false;
        }

        void applySettings() {

            static bool initialUpdate = true;

            setDeviceName(config.deviceName);

            for (int i=0; i<this->getNumMotors(); i++) {
                this->setMotorLimits(i,   ((double)config.pf[i].minPWM) / 100.0, 
                                        ((double)config.pf[i].maxPWM) / 100.0);
            }

            for (int i=0; i<this->getNumServos(); i++) {

                if (!initialUpdate)
                    this->servo(i)->detach();
                
                this->servo(i)->setServoMaxAngle(config.servo[i].maxAngle);
                
                if (!initialUpdate)
                    this->servo(i)->begin();
            }

            initialUpdate = false;        
        };

        void loadDefaultSettings() {
            config.loadDefaultSettings(this->getNumMotors(), this->getNumServos());
        };

        bool loadSettingsFromJson(JsonDocument &json) { 
            return config.loadSettingsFromJson(json); 
        };

        JsonDocument saveSettingsToJson() { 
            return config.saveSettingsToJson(); 
        }; 


};


#endif