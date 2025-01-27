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
#include <esp_timer.h>

#include "ButtonMonitor.h"
#include "GeekShield.h"

void taskButtonMonitor(void *param){
  ButtonMonitor::instance()->task();
}

void ButtonMonitor::setup() {

  config = GeekShield::instance()->getConfig();
 
  pinMode(config->pinAuxButton, INPUT);
  xTaskCreate(taskButtonMonitor, "ButtonMonitor", TASK_STACK_SIZE, NULL, 0, NULL);
}

void ButtonMonitor::task() {
  while (1) {
    vTaskDelayMillis(TASK_DEFAULT_DELAY_MILLIS);

    // wait until aux button is pressed
    while (analogReadMilliVolts(config->pinAuxButton) > config->adcLowValueMilliVolts) {
      vTaskDelayMillis(TASK_DEFAULT_DELAY_MILLIS);  
    }

    long time_pressed = esp_timer_get_time();
    long click_time = 0;
    bool long_click_handled = false;

    // wait until aux button is released
    while (analogReadMilliVolts(config->pinAuxButton) <= config->adcLowValueMilliVolts) {
      vTaskDelayMillis(10);  

      long time_now = esp_timer_get_time();
      click_time = (time_now - time_pressed) / 1000;

      if (click_time > config->buttonPressHold) {
        Serial.println("Click HOLD");
        GeekShield::instance()->resetPairing();
        break;
      }
      if (click_time > config->buttonPressLong) {
        if (!long_click_handled){
          long_click_handled = true;
          Serial.println("Click LONG");
          GeekShield::instance()->enablePairing();
        }
      }
    }

    if (click_time < config->buttonPressShort) {
      Serial.println("Click SHORT");
      if (config->buttonProfileSwitching)
        GeekShield::instance()->switchActiveProfile();
    }

    // ensure button is released before waiting for next click
    while (analogReadMilliVolts(config->pinAuxButton) <= config->adcLowValueMilliVolts) {
      vTaskDelayMillis(TASK_DEFAULT_DELAY_MILLIS);
    }
  }    
}