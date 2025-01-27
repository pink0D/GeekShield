//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#ifndef _GEEK_SHIELD_CONFIG_H_
#define _GEEK_SHIELD_CONFIG_H_

struct GeekShieldConfig { 
  
  int pinVoltage = 0;
  int pinAuxButton = 0;
  int pinPowerOff = 2;
  int pinLED = 4;

  int pinMotorA1 = 14;
  int pinMotorA2 = 15;
  int pinMotorB1 = 13;
  int pinMotorB2 = 12;

  int pinServo1 = 16;
  int pinServo2 = 12;
  int pinServo3 = 13;
  int pinServo4 = 15;
  int pinServo5 = 14;

  double ledBrightness = 0.005;
  double ledFrequency  = 1000;

  long adcLowValueMilliVolts = 500;
  bool adcAdditionalCalibration = false;
  double adcV1_true     = 1.3;
  double adcV1_reading  = 1.3;  
  double adcV2_true     = 2.6;
  double adcV2_reading  = 2.6;  

  long buttonPressShort = 500;     
  long buttonPressLong  = 1000;    
  long buttonPressHold  = 10000;

  bool buttonProfileSwitching = true;

  double  batteryAdcScale             = 2.75;

  int     batteryCellCount            = 2;
  double  batteryVoltageCutoff        = 3.2;
  double  batteryVoltageWarning       = 3.4;

  long    batteryCheckTaskDelayMillis      = 500;
  int     batteryCheckSampleCount          = 5;
  int     batteryCheckSampleIntervalMillis = 10;

  double pfMotorPwmFrequency   = 1000;
  double pfMotorPwmMinimumDuty = 0.25;

  double motorAntiJitterDefault = 0.05;

  int controllerStickDeadzoneLow = 50; 
  int controllerStickDeadzoneHigh = 500;

  int controllerTriggerDeadzoneLow = 5;
  int controllerTriggerDeadzoneHigh = 1000;

  long controllerTimeout      = 500;
  long idleTimeout = 300000;
};

#endif