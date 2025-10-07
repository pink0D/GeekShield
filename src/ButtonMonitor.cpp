//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>

#include "GeekShield.h"
#include "ButtonMonitor.h"

void taskButtonMonitor(void *param){
  ButtonMonitor::instance()->task();
}

void ButtonMonitor::setup() {

  config = GeekShield::instance()->getConfig();
 
  if (config->pinAuxButton == config->pinVoltage) { // same pin used both for battery check and for button
    pinMode(config->pinAuxButton, INPUT);
    readPinFuncPtr = &ButtonMonitor::readAnalogPin;
  }
  else {
    pinMode(config->pinAuxButton, INPUT_PULLUP);
    readPinFuncPtr = &ButtonMonitor::readDigitalPin;
  }

  xTaskCreate(taskButtonMonitor, "ButtonMonitor", TASK_STACK_SIZE, NULL, 0, NULL);
}

void ButtonMonitor::task() {

  // wait until button is released after power on
  while ( (this->*readPinFuncPtr)() == LOW) {
    vTaskDelayMillis(TASK_DEFAULT_DELAY_MILLIS);  
  }

  while (1) {
    vTaskDelayMillis(TASK_DEFAULT_DELAY_MILLIS);

    // wait until aux button is pressed
    while ( (this->*readPinFuncPtr)() == HIGH ) {
      vTaskDelayMillis(TASK_DEFAULT_DELAY_MILLIS);  
    }

    long time_pressed = esp_timer_get_time();
    long click_time = 0;
    bool short_click_handled = false;
    bool long_click_handled = false;

    // wait until aux button is released
    while ( (this->*readPinFuncPtr)() == LOW ) {
        vTaskDelayMillis(10);  

      long time_now = esp_timer_get_time();
      click_time = (time_now - time_pressed) / 1000;

      if (click_time >= config->buttonPressShort) {
        if (!short_click_handled){
          short_click_handled = true;
          Serial.println("Click SHORT start");
          GeekShield::instance()->processButtonEvent(ClickType::Short, EventType::Press);
        }
      }

      if (click_time >= config->buttonPressLong) {
        if (!long_click_handled){
          long_click_handled = true;
          Serial.println("Click LONG start");
          GeekShield::instance()->processButtonEvent(ClickType::Long, EventType::Press);
        }
      }

      if (click_time >= config->buttonPressHold) {
        Serial.println("Click HOLD start");
        GeekShield::instance()->processButtonEvent(ClickType::Hold, EventType::Press);
        break;
      }
    }

    if ( (click_time >= config->buttonPressShort) && (click_time < config->buttonPressLong) ){
      Serial.println("Click SHORT end");
      GeekShield::instance()->processButtonEvent(ClickType::Short, EventType::Release);
    }
    if ( (click_time >= config->buttonPressLong) && (click_time < config->buttonPressHold) ){
      Serial.println("Click LONG end");
      GeekShield::instance()->processButtonEvent(ClickType::Long, EventType::Release);
    }
    if (click_time >= config->buttonPressHold){
      Serial.println("Click HOLD end");
      GeekShield::instance()->processButtonEvent(ClickType::Hold, EventType::Release);
    }

    // ensure button is released before waiting for next click
    while ( (this->*readPinFuncPtr)() == LOW ) {
      vTaskDelayMillis(TASK_DEFAULT_DELAY_MILLIS);
    }
  }    
}

int ButtonMonitor::readDigitalPin() {
  return digitalRead(config->pinAuxButton);
}

int ButtonMonitor::readAnalogPin() {
  if (analogReadMilliVolts(config->pinAuxButton) > config->adcLowValueMilliVolts)
    return HIGH;

  return LOW;
}
