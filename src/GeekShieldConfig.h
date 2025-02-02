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
  
  /*
  PCB pin mappings 
  */

  int pinVoltage = 0;   // not an error, this is GPIO0
  int pinAuxButton = 0; // not an error, this is GPIO0
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

  /*
  LED indication (flash) parameters
  */

  double ledBrightness = 0.005;
  double ledFrequency  = 1000;

  /*
  Values for additional ADC calibration. 
  Unfortunately this doesn't make readings more accurate
  */

  // this value however is also used to treat analog ADC readings as logical LOW
  long adcLowValueMilliVolts = 500;       

  bool adcAdditionalCalibration = false;
  double adcV1_true     = 1.3;
  double adcV1_reading  = 1.3;  
  double adcV2_true     = 2.6;
  double adcV2_reading  = 2.6;  

  /*
  Button timings (milliseconds)
  */

  bool buttonMonitorEnable = true; // set this to 'false' if the PCB has no button

  long buttonPressShort = 50;     
  long buttonPressLong  = 1000;    
  long buttonPressHold  = 10000;

  /*
  Battery charge monitoring
  */

  bool batteryMonitorEnable = true; // set this to 'false' to disable battery monitoring

  // this is the most important parameter, if it is not set explicitly during setup, the board will power off immidiately
  double  batteryAdcScale             = 0; 

  int     batteryCellCount            = 2;
  double  batteryVoltageCutoff        = 3.2;  // absolute minimum battery cell voltage - the board will be powered off
  double  batteryVoltageWarning       = 3.4;  // battery level when motors are disabled

  long    batteryCheckTaskDelayMillis      = 500;
  int     batteryCheckSampleCount          = 5;
  int     batteryCheckSampleIntervalMillis = 10;

  /*
  Power Functions parameters
  */

  double pfMotorPwmFrequency   = 1000;  // motor PWM frequency
  double pfMotorPwmMinimumDuty = 0.25;  // minimum duty cycle

  double motorAntiJitterDefault = 0.05; // minimum change in duty cycle that is sent to motors - used to avoid servo jittering

  /*
  Controller input adjustments
  */

  int controllerStickDeadzoneLow = 50; 
  int controllerStickDeadzoneHigh = 500;

  int controllerTriggerDeadzoneLow = 5;
  int controllerTriggerDeadzoneHigh = 1000;

  /*
  Timeouts (milliseconds)
  */

  long idleTimeout       = 300000; // power off when there's no input
  long controllerTimeout = 500;    // stop motors when no data from controller is received (possibly out of range)
  bool controllerAutoDisconnect = false; // drop BT connection if no controller data received
};

#endif