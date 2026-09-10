#include <Arduino.h>

#define BUTTON_PIN 15
#define DEBOUNCE_TIME_MS 50

volatile uint32_t g_rawInterruptCount = 0;
volatile bool g_buttonFlag = false;

void IRAM_ATTR buttonIsr() {
    g_rawInterruptCount++;
    g_buttonFlag = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonIsr, FALLING);
}

void loop() {
    static uint32_t lastValidPressTime = 0;
    static uint32_t validPressCount = 0;

    if (g_buttonFlag) {
        g_buttonFlag = false;
        const uint32_t now = millis();

        if (now - lastValidPressTime >= DEBOUNCE_TIME_MS) {
            lastValidPressTime = now;
            validPressCount++;
            Serial.printf("Valid Press #%u | (Raw ISR Count: %u)\n", 
                          validPressCount, g_rawInterruptCount);
        }
    }
}
