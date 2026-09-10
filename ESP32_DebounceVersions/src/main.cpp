#include <Arduino.h>

#define BUTTON_PIN 15

volatile uint32_t g_interruptCount = 0;
volatile bool g_isrFlag = false;

void IRAM_ATTR buttonIsr() {
    g_interruptCount++;
    g_isrFlag = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonIsr, FALLING);
}

void loop() {
    if (g_isrFlag) {
        g_isrFlag = false;
        Serial.printf("Raw Interrupt! Total ISR calls: %u\n", g_interruptCount);
    }
}
