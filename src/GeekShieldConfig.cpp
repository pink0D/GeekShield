//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "GeekShieldConfig.h"

#include <map>
#include <algorithm>

static const char* configVersion = "DSv1";

static const std::map<GeekShieldConfig::ControllerButton, std::string> mapButtons = {
    {GeekShieldConfig::ControllerButton::NullButton, ""}, 
    {GeekShieldConfig::ControllerButton::DpadUp, "DpadUp"},
    {GeekShieldConfig::ControllerButton::DpadDown, "DpadDown"},
    {GeekShieldConfig::ControllerButton::DpadLeft, "DpadLeft"}, 
    {GeekShieldConfig::ControllerButton::DpadRight, "DpadRight"}, 
    {GeekShieldConfig::ControllerButton::L1, "L1"},
    {GeekShieldConfig::ControllerButton::R1, "R1"}, 
    {GeekShieldConfig::ControllerButton::L3, "L3"}, 
    {GeekShieldConfig::ControllerButton::R3, "R3"}, 
    {GeekShieldConfig::ControllerButton::Square, "Square"}, 
    {GeekShieldConfig::ControllerButton::Triangle, "Triangle"}, 
    {GeekShieldConfig::ControllerButton::Cross, "Cross"}, 
    {GeekShieldConfig::ControllerButton::Circle, "Circle"}
};

static const std::map<GeekShieldConfig::ControllerInput, std::string> mapInputs = {
    {GeekShieldConfig::ControllerInput::NullInput, ""},
    {GeekShieldConfig::ControllerInput::L2, "L2"},
    {GeekShieldConfig::ControllerInput::R2, "R2"},
    {GeekShieldConfig::ControllerInput::LeftStickX, "LeftStickX"}, 
    {GeekShieldConfig::ControllerInput::LeftStickY, "LeftStickY"}, 
    {GeekShieldConfig::ControllerInput::RightStickX, "RightStickX"}, 
    {GeekShieldConfig::ControllerInput::RightStickY, "RightStickY"}
};

std::string GeekShieldConfig::mapControllerButtonToString(GeekShieldConfig::ControllerButton btn) {

    if (mapButtons.count(btn) > 0)
        return mapButtons.at(btn);

    return "";
}

GeekShieldConfig::ControllerButton GeekShieldConfig::mapStringToControllerButton(std::string str) {

    auto it = std::find_if(mapButtons.begin(), mapButtons.end(), 
        [&str](const std::pair<GeekShieldConfig::ControllerButton, std::string>& pair) {
            return pair.second == str;
        });

    if (it != mapButtons.end()) 
        return it->first;

    return GeekShieldConfig::ControllerButton::NullButton;
}

std::string GeekShieldConfig::mapControllerInputToString(GeekShieldConfig::ControllerInput input) {

    if (mapInputs.count(input) > 0)
        return mapInputs.at(input);

    return "";
}

GeekShieldConfig::ControllerInput GeekShieldConfig::mapStringToControllerInput(std::string str) {
    auto it = std::find_if(mapInputs.begin(), mapInputs.end(), 
        [&str](const std::pair<GeekShieldConfig::ControllerInput, std::string>& pair) {
            return pair.second == str;
        });

    if (it != mapInputs.end()) 
        return it->first;

    return GeekShieldConfig::ControllerInput::NullInput;
}

void GeekShieldConfig::loadDefaultSettings(int numMotors, int numServos) {

    deviceName = "GeekShield";

    for (int i=0; i<numMotors; i++) {
        
        PFPort pfport;
        pfport.minPWM = 25;
        pfport.maxPWM = 100;
        pfport.input = ControllerInput::NullInput;
        pfport.brake = ControllerInput::NullInput;
        pfport.invertInput = false;
        pfport.invertButton = ControllerButton::NullButton;

        pf.push_back(pfport);
    }

    for (int i=0; i<numServos; i++) {
        ServoPort servoport;

        servoport.maxAngle = 90;
        servoport.input = ControllerInput::NullInput;
        servoport.invertInput = false;
        servoport.invertButton = ControllerButton::NullButton;

        servo.push_back(servoport);
    }
}

JsonDocument GeekShieldConfig::saveSettingsToJson() {
    
    JsonDocument json, deviceSettings, motors, servos;

    deviceSettings["deviceName"] = deviceName;

    for (int i=0; i<pf.size(); i++) {

        PFPort pfport = pf[i];

        JsonDocument motor;
        motor["input"] = mapControllerInputToString(pfport.input);
        motor["brake"] =  mapControllerInputToString(pfport.brake);
        motor["invertInput"] = pfport.invertInput;
        motor["invertButton"] = mapControllerButtonToString(pfport.invertButton);
        //motor["minPWM"] = pfport.minPWM;
        //motor["maxPWM"] = pfport.maxPWM;

        motors[i] = motor;
    }

    for (int i=0; i<servo.size(); i++) {

        ServoPort servoport = servo[i];

        JsonDocument servo;
        servo["input"] = mapControllerInputToString(servoport.input);
        servo["invertInput"] = servoport.invertInput;
        servo["invertButton"] = mapControllerButtonToString(servoport.invertButton);
        servo["maxAngle"] = servoport.maxAngle;

        servos[i] = servo;
    }    

    json["version"] = configVersion;
    json["deviceSettings"] = deviceSettings;
    json["motors"] = motors;
    json["servos"] = servos;

    return json;
}

bool GeekShieldConfig::loadSettingsFromJson(JsonDocument &json) {

    std::string version = json["version"];

    if (version != configVersion)
        return false;

    for (int i=0; i<pf.size(); i++) {

        PFPort pfport = pf[i];

        pfport.input = mapStringToControllerInput(json["motors"][i]["input"]);
        pfport.brake = mapStringToControllerInput(json["motors"][i]["brake"]);
        pfport.invertInput = json["motors"][i]["invertInput"];
        pfport.invertButton = mapStringToControllerButton(json["motors"][i]["invertButton"]);
        //pfport.maxPWM = max(0, min(100,       (int)(json["motors"][i]["maxPWM"])));
        //pfport.minPWM = max(0, min(pf.maxPWM, (int)(json["motors"][i]["minPWM"])));

        pf[i] = pfport;
    }

    for (int i=0; i<servo.size(); i++) {

        ServoPort servoport = servo[i];

        servoport.input = mapStringToControllerInput(json["servos"][i]["input"]);
        servoport.invertInput = json["servos"][i]["invertInput"];
        servoport.invertButton = mapStringToControllerButton(json["servos"][i]["invertButton"]);
        servoport.maxAngle = max(1, min(180,  (int)(json["servos"][i]["maxAngle"])));

        servo[i] = servoport;
    }

    return true;
}

