#include <Arduino.h>

#define ADC_PIN    4

#define VREF_MV           3100.0  // Uref in mV
#define ADC_RESOLUTION    4095.0  // ADCmax

// Voltage calculations
float getCalcVoltage(int raw) {
  float voltage = 0.0f;
  voltage = ((float)raw / ADC_RESOLUTION) * VREF_MV;
  return voltage;  
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12); // 12 bits (0..4095)
  pinMode(ADC_PIN, INPUT);

  // Print the values to the Serial Monitor
  Serial.println("\nRAW | U_calc (mV) | U_meas (mV) | Error (%)");
}

void loop() {
  // Read the RAW value
  int rawValue = analogRead(ADC_PIN);

  // Calculating the voltage
  float uCalc = getCalcVoltage(rawValue);

  // Read the calibrated voltage
  uint32_t uMeas = analogReadMilliVolts(ADC_PIN);

  // Calculating the error
  float errorPct = 0.0;
  if (uMeas > 0) {
    errorPct = (fabs(uCalc - (float)uMeas) / (float)uMeas) * 100.0;
  }

  // Printing the results to the Serial Monitor
  Serial.printf("%4d | %11.2f | %11u | %8.2f%%\n", rawValue, uCalc, uMeas, errorPct);

  // Get the Raw value each 100ms
  delay(100);
}
