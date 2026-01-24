//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _GEEKSHIELD_CONFIG_H_
#define _GEEKSHIELD_CONFIG_H_

#include <vector>
#include <string>
#include <ArduinoJson.h>

class GeekShieldConfig {  

    public:

        GeekShieldConfig() {};

        void loadDefaultSettings(int numMotors, int numServos);
        bool loadSettingsFromJson(JsonDocument &json);
        JsonDocument saveSettingsToJson(); 

        enum class ControllerButton {NullButton, 
            DpadUp, DpadDown, DpadLeft, DpadRight, 
            L1, R1, L3, R3, 
            Square, Triangle, Cross, Circle};

        enum class ControllerInput {NullInput, L2, R2, LeftStickX, LeftStickY, RightStickX, RightStickY};

        class PFPort {

            public:
                PFPort() {};

                int minPWM;
                int maxPWM;

                ControllerInput input;
                ControllerInput brake;
                
                bool invertInput;
                ControllerButton invertButton;
        };

        class ServoPort {

            public:
                ServoPort() {};

                int maxAngle;

                ControllerInput input;
                
                bool invertInput;
                ControllerButton invertButton;
        };



        // begin parameters

        std::string deviceName;

        std::vector<PFPort> pf;
        std::vector<ServoPort> servo;

        // end parameters

    private:
        
        static std::string mapControllerButtonToString(ControllerButton btn);
        static ControllerButton mapStringToControllerButton(std::string str);

        static std::string mapControllerInputToString(ControllerInput input);
        static ControllerInput mapStringToControllerInput(std::string str);

};

#endif
