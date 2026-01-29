//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//


#ifndef _GEEKSHIELD_H_
#define _GEEKSHIELD_H_

#include <BluepadHub.h>
#include <MultiServoUnit.h>
#include <MultiMotorUnit.h>
#include <NeoPixelStatusIndicator.h>
#include <MotorDriverHBridge.h>
#include <ServoPWM.h>
#include <VoltageMonitor.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include "AnalogButton.h"
#include "LedIndicator.h"

#define SERVO_1 0
#define SERVO_2 1
#define SERVO_3 2
#define SERVO_4 3
#define SERVO_5 4
#define SERVO_6 5
#define SERVO_7 6
#define SERVO_8 7

#define MOTOR_A 0
#define MOTOR_B 1
#define MOTOR_C 2
#define MOTOR_D 3
#define MOTOR_E 4
#define MOTOR_F 5

///////////////////////////////////////////////////////////////
// GeekShield base class
///////////////////////////////////////////////////////////////

template <int numMotors, int numServos>
class GeekShieldBase : 
  protected bluepadhub::MultiServoUnit<numServos, bluepadhub::ServoPWM>, 
  protected bluepadhub::MultiMotorUnit<numMotors, bluepadhub::MotorDriverHBridge>,
  protected bluepadhub::VoltageMonitor,
  private bluepadhub::DeepSleep,
  public bluepadhub::Profile
{
  
  protected:

    GeekShieldBase() {};

    int adcPin = 0;
    double adcScale = 0;

    int getNumMotors() { return numMotors; };
    int getNumServos() { return numServos; };

    virtual void afterSetup(const int *motorPin1, const int *motorPin2, const int* servoPin, int adcPin, double adcScale) {
      
      this->adcPin = adcPin;

      if (adcScale > 0) {
        this->adcScale = adcScale;
      } else {
        Serial.println("WARNING: adcScale is not set, battery discharge protection will not work!");
      }

      ::BluepadHub.setDeepSleep(this);

      for (int i=0; i < numMotors; i++) {
        this->motor(i)->begin(motorPin1[i], motorPin2[i], 1000);
      }

      for (int i=0; i < numServos; i++) {
        this->servo(i)->begin(servoPin[i]);
      }      
    };

  public:

    void begin() {
      BluepadHub.begin();
    };

    void update() {
      BluepadHub.update();
    };

    void stop() {
      this->stopMotors();
      this->stopServos();
    };

  private:

    double readVoltage() override {       

      return analogReadMilliVolts(adcPin) * adcScale / 1000.0;;
    }; 

    void failsafe() override {
      stop();
    };

    bool isLowBatteryState() override {
      
      if (adcScale > 0)
        return isLowBattery();

      return false;
    };

    void idleTimer() override {
      // go to deep sleep
      BluepadHub.startDeepSleep();
    };

};

///////////////////////////////////////////////////////////////
// GeekShield 2.4
///////////////////////////////////////////////////////////////

class GeekShieldV2 : public GeekShieldBase<2,4> {
  public:
    GeekShieldV2() {};

  private:
    const int powerCtlPin = 20;
    const int ledPin = 13;
    const int buttonPin = 21;
    const int batteryVoltagePin = 37;

    const double batteryScale = 4.4;

  protected:

    bluepadhub::NeoPixelStatusIndicator indicator;
    bluepadhub::MultiFunctionButton button;
  
    void beforeSetup() final {

      pinMode(powerCtlPin, OUTPUT);    // power hold pin
      digitalWrite(powerCtlPin, HIGH);

      indicator.setBrightness(20);
      indicator.begin(ledPin);
    }

    void afterSetup() final {

      pinMode(batteryVoltagePin, INPUT); // ADC for voltage reading

      button.begin(500, 5000, 10000, buttonPin, INPUT_PULLUP);

      const int motorPin1[] = {33, 22};
      const int motorPin2[] = {32, 19};
      const int servoPin[] = {25, 26, 27, 14};

      GeekShieldBase<2,4>::afterSetup(motorPin1, motorPin2, servoPin, batteryVoltagePin, batteryScale);
    };

    void beforeSleep() final {
      digitalWrite(powerCtlPin, LOW);    // power hold pin
    };
};


///////////////////////////////////////////////////////////////
// GeekShield DIY
///////////////////////////////////////////////////////////////

template <int numServos, int numMotors>
class GeekShieldDIY : public GeekShieldBase<numMotors,numServos> {
  public:
    GeekShieldDIY() {};

  private:
    const int powerCtlPin = 2;
    const int ledPin = 4;
    const int buttonPin = 0;
    const int batteryVoltagePin = 0;

  protected:

    LedIndicator indicator;
    AnalogButton button;

    virtual double batteryAdcScale() = 0;

    void beforeSetup() final {

      WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

      pinMode(powerCtlPin, OUTPUT);    // power off pin
      digitalWrite(powerCtlPin, LOW);

      indicator.setBrightness(2);
      indicator.begin(ledPin);
    }
  
    void afterSetup() final {

      pinMode(batteryVoltagePin, INPUT); // ADC for voltage reading

      button.begin(500, 5000, 10000, buttonPin, INPUT);

      const int motorPin1[] = {14, 13};
      const int motorPin2[] = {15, 12};
      const int servoPin[] = {16, 12, 13, 15, 14};

      if ( (numMotors < 0) || (numServos < 0)
            || (numMotors > 2) || (numServos > 5) 
            || (numMotors*2 + numServos > 5) ) 
      {
        Serial.println("Invalid combination of numMotors and numServos for GeekShield");
        indicator.setErrorStatus();
      } else {
        GeekShieldBase<numMotors,numServos>::afterSetup(motorPin1, motorPin2, servoPin, batteryVoltagePin, batteryAdcScale());
      }
    };

    void beforeSleep() final {
      digitalWrite(powerCtlPin, HIGH);    // power off pin
    };
};

#endif
