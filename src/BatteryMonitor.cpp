//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <FreeRTOS/FreeRTOS.h>
#include <FreeRTOS/task.h>

#include "BatteryMonitor.h"
#include "GeekShield.h"

void taskBatteryMonitor(void *param){
  BatteryMonitor::instance()->task();
}

void BatteryMonitor::setup() {
  
  config = GeekShield::instance()->getConfig();

  pinMode(config->pinVoltage, INPUT);
  xTaskCreate(taskBatteryMonitor, "BatteryMonitor", TASK_STACK_SIZE, NULL, 0, NULL);
}

void BatteryMonitor::task() {
  while (1) {
    vTaskDelayMillis(config->batteryCheckTaskDelayMillis);
    
    double bat_voltage = readBatteryVoltage();
    Serial.printf("Battery voltage = %.3f V \n", bat_voltage);

    if (bat_voltage < config->batteryCellCount * config->batteryVoltageWarning) {
      Serial.println("Going to low power mode");
      GeekShield::instance()->lowBatteryWarning();
    }

    if (bat_voltage < config->batteryCellCount * config->batteryVoltageCutoff) {
      Serial.println("POWER OFF");
      GeekShield::instance()->powerOff();
    }

  }    
}

double BatteryMonitor::readBatteryVoltage() {

  double value_sum = 0;
  double num_samples = config->batteryCheckSampleCount;

  for (int i=0; i<config->batteryCheckSampleCount; i++) {
    vTaskDelayMillis(config->batteryCheckSampleIntervalMillis);

    double pin_value = 0;
    while(1) { 
      pin_value = analogReadMilliVolts(config->pinVoltage);
      if (pin_value > config->adcLowValueMilliVolts ) // value looks like battery voltage
        break;

      // aux button was pressed so pin was pulled down to GND, or ADC was busy
      vTaskDelayMillis(100); // retry later
    } 

    value_sum += pin_value;
  }

  if (num_samples > 0) {
    double pin_value_avg = value_sum / num_samples / 1000.0;
    Serial.printf("Pin voltage = %.3f V \n", pin_value_avg);
    return calibrateVoltage(pin_value_avg * config->batteryAdcScale);
  }

  return 0;
}

double BatteryMonitor::calibrateVoltage(double value) {
  
  if (!config->adcAdditionalCalibration)
    return value;
  
  double X1 = config->adcV1_reading;
  double Y1 = config->adcV1_true;
  double X2 = config->adcV2_reading;
  double Y2 = config->adcV2_true;

  return Y1 + (value - X1) * (Y2 - Y1) / (X2 - X1);
}