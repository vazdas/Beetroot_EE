#include <Arduino.h>

struct Config {
    static constexpr uint8_t BUTTON_PIN        = 15;
    static constexpr uint32_t POLL_INTERVAL_MS = 10;
    static constexpr uint32_t DEBOUNCE_TIME_MS = 50;
};

enum class ButtonState {
    IDLE,
    DEBOUNCE,
    PRESSED
};

void setup() {
    Serial.begin(115200);
    pinMode(Config::BUTTON_PIN, INPUT_PULLUP);
    Serial.println("Polling FSM Debounce Started (No ISR)...");
}

void loop() {
    static uint32_t lastPollTime = 0;
    static uint32_t stateChangeTime = 0;
    static uint32_t validPressCount = 0;
    static ButtonState currentState = ButtonState::IDLE;

    const uint32_t now = millis();

    if (now - lastPollTime >= Config::POLL_INTERVAL_MS) {
        lastPollTime = now;
        const bool isPressed = (digitalRead(Config::BUTTON_PIN) == LOW);

        switch (currentState) {
            case ButtonState::IDLE:
                if (isPressed) {
                    currentState = ButtonState::DEBOUNCE;
                    stateChangeTime = now;
                }
                break;

            case ButtonState::DEBOUNCE:
                if (now - stateChangeTime >= Config::DEBOUNCE_TIME_MS) {
                    if (isPressed) {
                        currentState = ButtonState::PRESSED;
                        validPressCount++;
                        Serial.printf("[Polling FSM] Clean Press #%u\n", validPressCount);
                    } else {
                        currentState = ButtonState::IDLE;
                    }
                }
                break;

            case ButtonState::PRESSED:
                if (!isPressed) {
                    currentState = ButtonState::IDLE;
                }
                break;
        }
    }
}
