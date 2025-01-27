//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "GeekShield.h"
#include "ControlProfile.h"

#include <nvs_flash.h>

ControlProfile::ControlProfile() {
  config = GeekShield::instance()->getConfig();
}

double ControlProfile::normalizeTriggerInput(int input) {

  if (input < config->controllerTriggerDeadzoneLow)
    return 0;

  if (input > config->controllerTriggerDeadzoneHigh)
    return 1;

  return ((double)input) / 1024.0 ;
}

double ControlProfile::normalizeStickInput(int input) {

  if (abs(input) < config->controllerStickDeadzoneLow)
    return 0;

  double sign = input > 0 ? 1.0 : -1.0;

  if (abs(input) > config->controllerStickDeadzoneHigh)
    return sign;

  return sign*((double)abs(input)) / 512.0;
}

void ControlProfile::initSettings(const char* nvs_namespace) {
  this->nvs_namespace = nvs_namespace;
}

int ControlProfile::getSettingInt(const char* nvs_key, int default_value) {
  return getSettingDouble(nvs_key, default_value);
}

void ControlProfile::updateSettingInt(const char* nvs_key, int new_value) {
  updateSettingDouble(nvs_key, new_value);
}

double ControlProfile::getSettingDouble(const char* nvs_key, double default_value) {
  nvs_handle_t nvs_handle;    
  esp_err_t err;

  err = nvs_open(nvs_namespace, NVS_READWRITE, &nvs_handle);
  if (err != ESP_OK) {
    Serial.println("NVS error opening storage");
    return default_value;
  }

  double value = default_value;
  size_t size = sizeof(value);
  err = nvs_get_blob(nvs_handle, nvs_key, &value, &size);
  if (err != ESP_OK && err != ESP_ERR_NVS_NOT_FOUND) {
    Serial.println("NVS error reading storage");
  }

  nvs_close(nvs_handle);

  Serial.printf("NVS read namespace = %s, key = %s, value = %.3f\n", nvs_namespace, nvs_key, value);
  return value;
}

void ControlProfile::updateSettingDouble(const char* nvs_key, double new_value) {

  nvs_handle_t nvs_handle;    
  esp_err_t err;

  err = nvs_open(nvs_namespace, NVS_READWRITE, &nvs_handle);
  if (err != ESP_OK) {
    Serial.println("NVS error opening storage");
    return;
  }

  double value = new_value;
  
  err = nvs_set_blob(nvs_handle, nvs_key, &value, sizeof(value));
  if (err != ESP_OK) { 
    Serial.println("NVS error writing storage");
    return;
  }

  err = nvs_commit(nvs_handle);
  if (err != ESP_OK) { 
    Serial.println("NVS error commiting storage");
    return;
  }

  nvs_close(nvs_handle);
  Serial.printf("NVS write namespace = %s, key = %s, value = %.3f\n", nvs_namespace, nvs_key, value);
}
