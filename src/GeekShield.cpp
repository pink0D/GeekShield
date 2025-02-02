//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "GeekShield.h"
#include "BatteryMonitor.h"
#include "ButtonMonitor.h"
#include "LedIndicator.h"

#include <esp_timer.h>
#include <nvs_flash.h>

GeekShield::GeekShield() {
  for (int i=0; i<MAX_PROFILES; i++)
    profiles[i] = nullptr;
}

void GeekShield::setup() {

  pinMode(config.pinPowerOff, OUTPUT);  digitalWrite(config.pinPowerOff, LOW);

  pinMode(config.pinServo1, OUTPUT);    digitalWrite(config.pinServo1, LOW);
  pinMode(config.pinServo2, OUTPUT);    digitalWrite(config.pinServo2, LOW);
  pinMode(config.pinServo3, OUTPUT);    digitalWrite(config.pinServo3, LOW);
  pinMode(config.pinServo4, OUTPUT);    digitalWrite(config.pinServo4, LOW);
  pinMode(config.pinServo5, OUTPUT);    digitalWrite(config.pinServo5, LOW);

  Serial.begin(115200);

  init_nvs();

  if (config.batteryMonitorEnable)
    BatteryMonitor::instance()->setup();
  
  if (config.buttonMonitorEnable)
    ButtonMonitor::instance()->setup();
  
  LedIndicator::instance()->setup();

  BP32.setup(&onConnectedController, &onDisconnectedController);
  //BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);
  BP32.enableNewBluetoothConnections(false); // pairing disabled by default

  LedIndicator::instance()->setStatusPattern(LedIndicator::StatusPattern::Idle);
}

void GeekShield::init_nvs(){
    esp_err_t ret;

    // initialize flash
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      Serial.println("Call nvs_flash_erase");
      ret = nvs_flash_erase();
      if(ret != ESP_OK) { 
        Serial.println("nvs_flash_erase error");
        return;
      }
    ret = nvs_flash_init();
  }
    if(ret != ESP_OK) { 
      Serial.println("nvs_flash_init error");
      return;
    }
}

void GeekShield::loop() {
  vTaskDelay(1);

  long time_now = esp_timer_get_time();

  // stop motors if controller is not sending updates (possible out of range)
  if ( (time_controller_update > 0) && (time_now > time_controller_update + config.controllerTimeout*1000) ){
    Serial.println("Controller timeout");
    time_controller_update = 0;
    stopMotors();
    if (config.controllerAutoDisconnect) {
      disconnectController();
      LedIndicator::instance()->setStatusPattern(LedIndicator::StatusPattern::Idle);
    }
  }

  // power off when idle for a long time
  if ( (time_auto_power_off > 0) && (time_now > time_auto_power_off) ){
    Serial.println("AUTO power off (idle)");
    time_auto_power_off = 0;
    powerOff();
  }

  // do nothing when in low power mode
  if (lowPowerMode)
    return;

  bool controllerUpdated = BP32.update();
  if (controllerUpdated)
  {
    if (bp32Controller && bp32Controller->isConnected() && bp32Controller->hasData()) {
      if (bp32Controller->isGamepad()) {
        if (profiles[activeProfile] != nullptr) {
          
          profiles[activeProfile]->processController(bp32Controller); 

          if (controllerCallback) { // check if a callback has been registered
            controllerCallback(bp32Controller); // call function pointer
          }  
          
          time_controller_update = esp_timer_get_time(); 

          // reset idle timeout when some input is present
          if (checkIdleMotors() == false) {
            time_auto_power_off = time_now + config.idleTimeout*1000;
          }
        }
      }
      else {
        Serial.println("BP32: disconecting controller since it is not a gamepad");
        disconnectController();
      }
    }
  }
}

void GeekShield::onConnectedController(ControllerPtr ctl) {
  if (instance()->bp32Controller == nullptr) {
    ControllerProperties properties = ctl->getProperties();
    Serial.printf("BP32: controller connected: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id, properties.product_id);
    instance()->bp32Controller = ctl;
    instance()->time_controller_update = 0;
    BP32.enableNewBluetoothConnections(false); // exit pairing mode after connection
    LedIndicator::instance()->setStatusPattern(LedIndicator::StatusPattern::Connected);
  } else {
    Serial.println("BP32: refusing new connection since another controller is already connected");
    ctl->disconnect();
  }
}

void GeekShield::onDisconnectedController(ControllerPtr ctl) {
    if (instance()->bp32Controller == ctl) {
        Serial.println("BP32: Controller disconnected");
        instance()->bp32Controller = nullptr;
    } else {
        Serial.println("BP32: Unknown controller disconnected");
    }
    LedIndicator::instance()->setStatusPattern(LedIndicator::StatusPattern::Idle);
}

void GeekShield::disconnectController() {
  if (bp32Controller != nullptr) {
    bp32Controller->disconnect();
    bp32Controller = nullptr;
    Serial.println("BP32: Controller disconnected");
  }
}

void GeekShield::enablePairing() {
  BP32.enableNewBluetoothConnections(true);
  LedIndicator::instance()->setStatusPattern(LedIndicator::StatusPattern::Pairing);
}

void GeekShield::resetPairing() {
  disconnectController();
  BP32.forgetBluetoothKeys();
  LedIndicator::instance()->setEventPattern(LedIndicator::EventPattern::Reset);
}

void GeekShield::lowBatteryWarning() {
  lowPowerMode = true;
  stopMotors();
  disconnectController();
  LedIndicator::instance()->setStatusPattern(LedIndicator::StatusPattern::BatteryLow);
}

void GeekShield::powerOff() {
  disconnectController();
  digitalWrite(config.pinPowerOff, HIGH);
}

PFMotor* GeekShield::getPFMotor(Port port, PFMotor::PwmType pwmType) {

  switch (port) {

    case Port::MotorA:
      motorA.setup(config.pinMotorA1, config.pinMotorA2, pwmType);
      return &motorA;

    case Port::MotorB:
      motorB.setup(config.pinMotorB1, config.pinMotorB2, pwmType);
      return &motorB;
  }

  return nullptr;
}

GeekServo* GeekShield::getGeekServo(Port port, int servoMin, int servoMax) {

  switch (port) {

    case Port::Servo1:
      motorServo[0].setup(config.pinServo1, servoMin, servoMax);
      return &motorServo[0];

    case Port::Servo2:
      motorServo[1].setup(config.pinServo2, servoMin, servoMax);
      return &motorServo[1];

    case Port::Servo3:
      motorServo[2].setup(config.pinServo3, servoMin, servoMax);
      return &motorServo[2];

    case Port::Servo4:
      motorServo[3].setup(config.pinServo4, servoMin, servoMax);
      return &motorServo[3];

    case Port::Servo5:
      motorServo[4].setup(config.pinServo5, servoMin, servoMax);
      return &motorServo[4];
  }

  return nullptr;
}

void GeekShield::stopMotors() {
  motorA.updateMotor(0);
  motorB.updateMotor(0);

  for (int i=0; i<MAX_GEEKSERVOS; i++)
    motorServo[i].updateMotor(0);
}

bool GeekShield::checkIdleMotors() {
  double sum = 0;

  sum += abs(motorA.getControlValue());
  sum += abs(motorB.getControlValue());

  for (int i=0; i<MAX_GEEKSERVOS; i++)
    sum += abs(motorServo[i].getControlValue());

  if (sum > 0)
    return false;

  return true;
}

void GeekShield::releaseMotors() {
  motorA.release();
  motorB.release();

  for (int i=0; i<MAX_GEEKSERVOS; i++)
    motorServo[i].release();
}

void GeekShield::addProfile(ControlProfile *profile) {
  for (int i=0; i<MAX_PROFILES; i++){
    if (profiles[i] == nullptr) {
      profiles[i] = profile;
      numProfiles++;
      if (i==0) // the first added profile is activeted by default
        profile->setup(this);
      return;
    }
  }

  Serial.println("No more profiles can be added");
}

void GeekShield::switchActiveProfile() {

  if (numProfiles < 2)
    return;

  Serial.println("Switch profile");

  // stop motors & release allocated PWM's by current profile
  stopMotors();
  releaseMotors();

  // cycle to next profile
  activeProfile += 1;
  if ( (activeProfile == MAX_PROFILES) || (profiles[activeProfile] == nullptr) )
    activeProfile = 0;

  // setup new profile
  if (profiles[activeProfile] != nullptr) {
    profiles[activeProfile]->setup(this);
  }

  LedIndicator::instance()->setEventPattern(LedIndicator::EventPattern::ProfileSelect);
}
