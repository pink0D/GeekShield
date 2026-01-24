/*
This sketch can be used to read and display voltage value from ESP32 pin

ATTENTION! 
Maximum pin voltage is 3.3V, so 2S battery must not be connected directly to ESP32 pin!

Additionally, 2-point calibration can be made to make readings more accurate
*/

#define VOLTAGE_PIN   0

#define SAMPLE_COUNT  5
#define SAMPLE_DELAY  10
#define TASK_DELAY    1000
#define ADC_MIN_VALUE_MILLIS 500

/*
  CALIBRATION DATA
*/
#define V1_ESP    1.2 // V1 voltage value reading by ESP32 ADC
#define V1_TRUE   1.2 // V1 true value measured externally on the same pin
#define V2_ESP    2.4 // V2 voltage value reading by ESP32 ADC
#define V2_TRUE   2.4 // V2 true value measured externally on the same pin


void vTaskDelayMillis(int ms) {
   vTaskDelay(pdMS_TO_TICKS(ms));
}

void setup() {
  pinMode(VOLTAGE_PIN, INPUT);

  Serial.begin(115200);
}

double readPinVoltage() {

  double value_sum = 0;
  double num_samples = SAMPLE_COUNT;

  for (int i=0; i<SAMPLE_COUNT; i++) {
    vTaskDelayMillis(SAMPLE_DELAY);

    double pin_value = 0;
    while(1) { 
      pin_value = analogReadMilliVolts(VOLTAGE_PIN);
      if (pin_value > ADC_MIN_VALUE_MILLIS
       ) // value looks like battery voltage
        break;

      // aux button was pressed so pin was pulled down to GND, or ADC was busy
      vTaskDelayMillis(100); // retry later
    } 

    value_sum += pin_value;
  }

  if (num_samples > 0) {
    return value_sum / num_samples / 1000.0;
  }

  return 0;
}

double calibrateVoltage(double value) {
  double X1 = V1_ESP;
  double Y1 = V1_TRUE;
  double X2 = V2_ESP;
  double Y2 = V2_TRUE;
  return Y1 + (value - X1) * (Y2 - Y1) / (X2 - X1);
}

void loop() {
  vTaskDelayMillis(TASK_DELAY);

  double voltage_raw = readPinVoltage();
  double voltage_calibrated = calibrateVoltage(voltage_raw);

  Serial.printf("Voltage: raw = %.3f V, calibrated = %.3f V\n", voltage_raw, voltage_calibrated);
}
