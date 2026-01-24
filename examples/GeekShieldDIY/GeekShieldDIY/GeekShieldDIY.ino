//
// Copyright (c) Dmitry Akulov. All rights reserved.
//
// Repository info:     https://github.com/pink0D/GeekShield
// Contact information: pink0D.github@gmail.com
//
// Licensed under the MIT license. See LICENSE file in the project root for details.
//

#include <GeekShield.h>

// possible combinations for GeekShieldDIY<numServos,numMotors>:
// GeekShieldDIY<1,2> - SERVO_1 + MOTOR_A + MOTOR_B
// GeekShieldDIY<3,1> - SERVO_1 + SERVO_2 + SERVO_3 + MOTOR_A
// GeekShieldDIY<5,0> - SERVO_1 + SERVO_2 + SERVO_3 + SERVO_4 + SERVO_5
class : public GeekShieldDIY<1,2> {

  double batteryAdcScale() {
    // modify this value so battery voltage measured by ESP32 matches the value on multimeter
    return 3.5;
  };

  void setup() {

    // automatically start pairing every time the controller is started
    // by default, pairing mode is activated by holding button for 5 secs or 10secs to forget pairing

    enablePairingAfterStartup = true;


    // uncomment to adjust controller sensivity
    //
    // controllerStickDeadzoneLow = 50;        // 0 = lowest value for stick input
    // controllerStickDeadzoneHigh = 500;      // 512 = highest value for stick input
    // controllerTriggerDeadzoneLow = 5;       // 0 = lowest value for trigger input
    // controllerTriggerDeadzoneHigh = 1000;   // 1024 = highest value for trigger input 

    // actual deadzone values depend on type of controller used
    // TestBluePad32 example can be used to analyze raw values sent by controller
    

    // adjust LED brightness
    indicator.setBrightness(2);

    // print battery voltage to Serial
    printVoltage(true);

    // specify 500..2500us pulse for 360-servo
    // if no pulse range is specified, then default 1000..2000us will be used (180-servo)
    setServoPulseRange(SERVO_1, 500, 2500);

    // alternatevely, you can specify servo rotation angle in degrees
    // this will be recalculated to corresponding pulse range internally
    // setServoMaxAngle(SERVO_1, 45);

    // PWM duty cycle for motors can be limited from 0 to 100% (default is full range)
    // usually it is recommended to set minimum 25% for Power Function motors
    setMotorLimits(MOTOR_A, 0.25, 1.0);
    setMotorLimits(MOTOR_B, 0.25, 1.0);

    // if necessary, the upper PWM limit can be also specified to limit maximum motor power 
    // setMotorLimits(MOTOR_A, 0.25, 0.75);
    // setMotorLimits(MOTOR_B, 0.25, 0.75);

  };

  void processBluepadController(BluepadController* ctl) {
    
    static int motor1_direction = 1;
    static int motor2_direction = 1;

    // returns 'true' only once when button is pressed
    if (wasClicked(ctl->l1()))
      motor1_direction = -motor1_direction;

    if (wasClicked(ctl->r1()))
      motor2_direction = -motor2_direction;

    // updateServo/updateMotor methods set new values for servo/motor channels output with anti-jitter filtering
    // argument is expected to be a normalized value between -1.0 and +1.0
    // 1.0 = full speed forward / max servo rotation clockwise
    // -1.0 = full speed reverse / max servo rotation counter-clockwise
    // 0.0 stops motors / returns servo to zero position
    // 0.5 / 0.75 or any other double value = set motor speed / servo rotation to 50% / 75% / etc

    // normalizeStickInput/normalizeTriggerInput methods apply deadzone correction to raw values
    // then integer values from controller are mapped to normalized range (-1.0, 1.0) 

    updateServo(SERVO_1, normalizeStickInput(ctl->axisX()));

    updateMotor(MOTOR_A, motor1_direction * normalizeTriggerInput(ctl->brake()));
    updateMotor(MOTOR_B, motor2_direction * normalizeTriggerInput(ctl->throttle()));

    // for more details on controller data processing see TestBluePad32 example and Bluepad32 docs
  };

} shield;


//  Arduino setup function
void setup() {
  shield.begin();
}

// Arduino loop function
void loop() {
  shield.update();
  //no delay here because it's inside shield.update()
}
