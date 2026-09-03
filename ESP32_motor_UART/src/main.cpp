#include <Arduino.h>

struct Config {
    static constexpr uint8_t BTN_LOW_PIN  = 5;
    static constexpr uint8_t BTN_MID_PIN  = 6;
    static constexpr uint8_t BTN_HIGH_PIN = 7;
    static constexpr uint8_t UART_TX_PIN  = 17;
    static constexpr uint32_t BAUD_RATE   = 115200;
    static constexpr uint32_t DEBOUNCE_MS = 50;
};

// Байти ШІМ для трьох режимів
static constexpr uint8_t PWM_LOW  = 0x03; // ~30% Duty Cycle
static constexpr uint8_t PWM_MID  = 0x3F; // ~70% Duty Cycle
static constexpr uint8_t PWM_HIGH = 0xFF; // ~90% Duty Cycle

static uint8_t g_selectedPwmByte = PWM_LOW;

void setup() {
    pinMode(Config::BTN_LOW_PIN, INPUT_PULLUP);
    pinMode(Config::BTN_MID_PIN, INPUT_PULLUP);
    pinMode(Config::BTN_HIGH_PIN, INPUT_PULLUP);

    Serial1.begin(Config::BAUD_RATE, SERIAL_8N1, -1, Config::UART_TX_PIN);
}

void loop() {
    static uint32_t lastDebounceTime = 0;
    static bool lastLowState  = HIGH;
    static bool lastMidState  = HIGH;
    static bool lastHighState = HIGH;

    const uint32_t now = millis();

    // 1. Опитування кнопок за фронтом (зчитуємо момент НАТИСКАННЯ)
    if (now - lastDebounceTime > Config::DEBOUNCE_MS) {
        const bool currentLow  = digitalRead(Config::BTN_LOW_PIN);
        const bool currentMid  = digitalRead(Config::BTN_MID_PIN);
        const bool currentHigh = digitalRead(Config::BTN_HIGH_PIN);

        // Кнопка 1: Низькі оберти (перехід HIGH -> LOW)
        if (lastLowState == HIGH && currentLow == LOW) {
            g_selectedPwmByte = PWM_LOW;
            lastDebounceTime = now;
        }
        // Кнопка 2: Середні оберти (перехід HIGH -> LOW)
        else if (lastMidState == HIGH && currentMid == LOW) {
            g_selectedPwmByte = PWM_MID;
            lastDebounceTime = now;
        }
        // Кнопка 3: Високі оберти (перехід HIGH -> LOW)
        else if (lastHighState == HIGH && currentHigh == LOW) {
            g_selectedPwmByte = PWM_HIGH;
            lastDebounceTime = now;
        }

        lastLowState  = currentLow;
        lastMidState  = currentMid;
        lastHighState = currentHigh;
    }

    // 2. Безперервна відправка ШІМ у буфер UART
    if (Serial1.availableForWrite() > 0) {
        Serial1.write(g_selectedPwmByte);
    }
}