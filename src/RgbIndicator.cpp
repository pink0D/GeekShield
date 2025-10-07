//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include "RgbIndicator.h"
#include "GeekShield.h"

#include <Adafruit_NeoPixel.h>

void RgbIndicator::setup() {

  config = GeekShield::instance()->getConfig();

  pixels = new Adafruit_NeoPixel(1, config->pinLED, NEO_GRB + NEO_KHZ800);
  pixels->begin();
  pixels->setBrightness(255.0 * config->ledBrightness);

}

void RgbIndicator::setLedColor(unsigned long color) {
  pixels->clear();
  pixels->setPixelColor(0,color);  
  pixels->show();
}

void RgbIndicator::showStatusPattern() {

  if (statusPattern == StatusPattern::Idle) {
    setLedColor(0x00FF00);  
    vTaskDelayMillis(500);
    setLedColor(0);  
    patternDelayMillis(500);
  }
  
  if (statusPattern == StatusPattern::Pairing) {
    setLedColor(0x0000FF);  
    vTaskDelayMillis(500);
    setLedColor(0);  
    patternDelayMillis(500);   
  }

  if (statusPattern == StatusPattern::Connected) {
    setLedColor(0x0000FF);  
    patternDelayMillis(500);
  }

  if (statusPattern == StatusPattern::BatteryLow) {
    setLedColor(0xFFA500);  
    vTaskDelayMillis(500);
    setLedColor(0);  
    patternDelayMillis(500);   
  }

  if (statusPattern == StatusPattern::Settings) {
    setLedColor(0xFFC0CB);  
    vTaskDelayMillis(500);
    setLedColor(0);  
    patternDelayMillis(500);   
  }     
}

void RgbIndicator::showEventPattern() {

  if (eventPattern == EventPattern::PowerOff) {
    setLedColor(0xFF0000);  
    vTaskDelayMillis(500);
    setLedColor(0);  
    vTaskDelayMillis(5000);
  }

  if (eventPattern == EventPattern::Reset) {
    for (int i=0; i<5; i++) {
      setLedColor(0xFF0000);  
      vTaskDelayMillis(200);
      setLedColor(0);  
      vTaskDelayMillis(200);
    }
  }

  if (eventPattern == EventPattern::ProfileSelect) {

    unsigned long color = GeekShield::instance()->getActiveProfileDisplayColor();
    
    if (color > 0) {
      setLedColor(color);  
      vTaskDelayMillis(1000);    
    } else {
      setLedColor(0xFFFFFF);  
      vTaskDelayMillis(1000);    
    }
  }
}
