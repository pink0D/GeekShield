//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "LedIndicator.h"
#include "RgbIndicator.h"
#include "GeekShield.h"

void taskLedIndicator(void *param){
  LedIndicator *inst = LedIndicator::instance();
  while (1) {
    vTaskDelay(1);
    inst->showStatusPattern();
  }
}

LedIndicator* LedIndicator::instance() {
    
  LedIndicator *inst = nullptr;

  if (GeekShield::instance()->getConfig()->ledRGB)
    inst = RgbIndicator::instance();
  else 
    inst = SimpleIndicator::instance();

  static bool taskCreated = false;
  if (!taskCreated) 
  {
    Serial.println("led task create");
    xTaskCreate(taskLedIndicator, "LedIndicator", TASK_STACK_SIZE, NULL, 0, NULL);
    taskCreated = true;
  }

  return inst;
}

void LedIndicator::patternDelayMillis(int timeout) {
  int time_to_wait = timeout - TASK_DEFAULT_DELAY_MILLIS;
  while (time_to_wait > 0){
    vTaskDelayMillis(TASK_DEFAULT_DELAY_MILLIS);
    time_to_wait -= TASK_DEFAULT_DELAY_MILLIS;

    if (eventPattern != EventPattern::None) {

      showEventPattern();
      eventPattern = EventPattern::None;
    }
  }
}

/*
  Simple monochome LED
*/

void SimpleIndicator::setup() {

  config = GeekShield::instance()->getConfig();

  pinMode(config->pinLED, OUTPUT);
  digitalWrite(config->pinLED, LOW);

  pwm.attachPin(config->pinLED, config->ledFrequency, 12);
}

void SimpleIndicator::setLedLevel(uint8_t val){
  pwm.writeScaled( ( ( (double)val)  / 255.0 ) * config->ledBrightness);
}

void SimpleIndicator::showStatusPattern() {
  switch (statusPattern) {
    case StatusPattern::None:
      setLedLevel(0);   
      break;
    case StatusPattern::Idle:
      //setLedLevel(255); vTaskDelayMillis(750);
      for (int i=0; i<256; i++) {
        setLedLevel(i);
        vTaskDelayMillis(10);
      }
      setLedLevel(0);   
      patternDelayMillis(750);
      break;
    case StatusPattern::Connected:
      setLedLevel(255); vTaskDelayMillis(200);
      setLedLevel(0);   
      patternDelayMillis(5000);
      break;
    case StatusPattern::Pairing:
      setLedLevel(255); vTaskDelayMillis(100);
      setLedLevel(0);   vTaskDelayMillis(100);
      setLedLevel(255); vTaskDelayMillis(100);
      setLedLevel(0);   
      patternDelayMillis(2000);
      break;
    case StatusPattern::BatteryLow:
      setLedLevel(255); vTaskDelayMillis(200);
      setLedLevel(0);   
      patternDelayMillis(5000);
      break;
    case StatusPattern::Settings:
      setLedLevel(255); vTaskDelayMillis(200);
      setLedLevel(0);   
      patternDelayMillis(200);
      break;
  }
}

void SimpleIndicator::showEventPattern() {
  switch (eventPattern) {
    case EventPattern::Reset:
      setLedLevel(255); vTaskDelayMillis(100);
      setLedLevel(0);   vTaskDelayMillis(100);
      setLedLevel(255); vTaskDelayMillis(100);
      setLedLevel(0);   vTaskDelayMillis(100);
      setLedLevel(255); vTaskDelayMillis(100);
      setLedLevel(0);   vTaskDelayMillis(100);
      setLedLevel(255); vTaskDelayMillis(100);
      setLedLevel(0);   vTaskDelayMillis(100);
      break;      
    case EventPattern::ProfileSelect:
      for (int i=0; i<GeekShield::instance()->getActiveProfileDisplayIndex(); i++) {
        setLedLevel(255); vTaskDelayMillis(200);
        setLedLevel(0);   vTaskDelayMillis(200);
      }
      break;
    case EventPattern::SettingSaved:
        setLedLevel(255); vTaskDelayMillis(200);
        setLedLevel(0);   vTaskDelayMillis(200);
        setLedLevel(255); vTaskDelayMillis(200);
        setLedLevel(0);   vTaskDelayMillis(200);
      break;
  }      
}
