#include <Arduino.h>

struct Config {
    static constexpr uint8_t POT_PIN         = 4;    //Analog input pin
    static constexpr uint8_t MOTOR_PIN       = 17;   //Digital output pin for motor
    static constexpr uint32_t PWM_PERIOD_US  = 2000;
};

void setup() {
    Serial.begin(115200);
    analogReadResolution(12); // 0..4095
    pinMode(Config::POT_PIN, INPUT);
    pinMode(Config::MOTOR_PIN, OUTPUT);
}

void loop() {
    //Getting the analog value from potentiometer
    const uint16_t adcRaw = analogRead(Config::POT_PIN);

    //Calculate the duration of the high level in microseconds
    const uint32_t highTimeUs = (static_cast<uint32_t>(adcRaw) * Config::PWM_PERIOD_US) / 4095;

    //PWM generation using micros()
    static uint32_t cycleStartUs = 0;
    const uint32_t currentUs = micros();

    
    if (currentUs - cycleStartUs >= Config::PWM_PERIOD_US) {
        cycleStartUs = currentUs;
    }

    
    const uint32_t elapsedTimeUs = currentUs - cycleStartUs;
    if (elapsedTimeUs < highTimeUs) {
        digitalWrite(Config::MOTOR_PIN, HIGH);
    } else {
        digitalWrite(Config::MOTOR_PIN, LOW);
    }
}