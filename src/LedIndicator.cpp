//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "LedIndicator.h"
#include <BluepadHub.h>

void LedIndicator::begin(int16_t pin, bool setBluepadHubStatusIndicator) {

  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);

  pwm.attachPin(pin, 1000, 12);

  StatusIndicator::begin(setBluepadHubStatusIndicator);
}

void LedIndicator::setBrightness(uint8_t _ledBrightness) {
  ledBrightness = _ledBrightness;
}

void LedIndicator::clear() {
  setLedLevel(0);
}

void LedIndicator::setLedLevel(double level) {
    
  pwm.writeScaled( ( ( (double)level)  / 255.0 ) * ledBrightness);
}

void LedIndicator::showStatusPattern() {

  if (statusPattern == StatusPattern::PowerOff) {
    setLedLevel(0);  
    patternDelayMillis(100);
  }

  if (statusPattern == StatusPattern::Idle) {
    setLedLevel(1); 
    vTaskDelayMillis(500);
    setLedLevel(0);  
    patternDelayMillis(1000);
  }
  
  if ( (statusPattern == StatusPattern::Error) || (statusPattern == StatusPattern::Warning) ){
    for (int i=0; i<5; i++) {
      setLedLevel(1); 
      vTaskDelayMillis(200);
      setLedLevel(0);  
      vTaskDelayMillis(200);      
    }
    
    patternDelayMillis(2000);   
  }

  if (statusPattern == StatusPattern::Pairing) {
    setLedLevel(1); 
    vTaskDelayMillis(250);
    setLedLevel(0);  
    patternDelayMillis(250);
  }

  if (statusPattern == StatusPattern::Connected) {
    setLedLevel(1); 
    vTaskDelayMillis(200);
    setLedLevel(0);  
    patternDelayMillis(2000);
  }
}

void LedIndicator::showEventPattern() {

  if (eventPattern == EventPattern::Reset) {
    for (int i=0; i<5; i++) {
      setLedLevel(1); 
      vTaskDelayMillis(100);
      setLedLevel(0);  
      vTaskDelayMillis(100);      
    }
  }

  if (eventPattern == EventPattern::Select) {
    setLedLevel(1); 
    vTaskDelayMillis(200);
    setLedLevel(0);  
    vTaskDelayMillis(200); 
  }
}
