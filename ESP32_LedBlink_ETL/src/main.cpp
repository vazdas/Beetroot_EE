#include <Arduino.h>

//Enumerators for LED state
enum class LedState : uint8_t {
    Off = 0,
    On = 1
};

//Enumerators for LED operation modes
enum class OperationMode : uint8_t {
    Blinking = 0,
    AlwaysOn = 1,
    AlwaysOff = 2
};

struct Config {
    static constexpr uint8_t LED_PIN = 15;
    static constexpr uint8_t BUTTON_PIN = 16;
    static constexpr uint32_t BLINK_INTERVAL_MS = 500;
    static constexpr uint32_t DEBOUNCE_MS = 50;    
};

//The class for LED
class Led {
private:
    uint8_t m_pin;
    LedState m_state;

public:
    explicit constexpr Led(uint8_t pin) 
        : m_pin(pin), m_state(LedState::Off) {}

    void init() {
        pinMode(m_pin, OUTPUT);
        set(LedState::Off);
    }

    void set(LedState state) {
        m_state = state;
                
        if (m_state == LedState::On) {
            digitalWrite(m_pin, HIGH);
        } else {
            digitalWrite(m_pin, LOW);
        }
    }

    void toggle() {        
        if (m_state == LedState::On) {
            set(LedState::Off);
        } else {
            set(LedState::On);
        }
    }
};

//check the state of a button
volatile bool g_buttonPressed = false;

void IRAM_ATTR buttonIsr() {
    g_buttonPressed = true;
}

static Led g_led(Config::LED_PIN);
static OperationMode g_currentMode = OperationMode::Blinking;

void setup() {
    Serial.begin(115200);
    g_led.init();

    pinMode(Config::BUTTON_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(Config::BUTTON_PIN), buttonIsr, RISING);
}

void loop() {
    //Handling button interrupts inside loop
    if (g_buttonPressed) {
        g_buttonPressed = false;

        static uint32_t lastInterruptTime = 0;
        const uint32_t now = millis();

        //Debounce
        if (now - lastInterruptTime > Config::DEBOUNCE_MS) {
            //Switching the modes
            switch (g_currentMode) {
                case OperationMode::Blinking:
                    g_currentMode = OperationMode::AlwaysOn;
                    break;
                case OperationMode::AlwaysOn:
                    g_currentMode = OperationMode::AlwaysOff;
                    break;
                case OperationMode::AlwaysOff:
                    g_currentMode = OperationMode::Blinking;
                    break;
            }
            lastInterruptTime = now;
        }
    }

    
    static uint32_t lastBlinkTime = 0;
    const uint32_t now = millis();

    switch (g_currentMode) {
        case OperationMode::Blinking:
            if (now - lastBlinkTime >= Config::BLINK_INTERVAL_MS) {
                lastBlinkTime = now;
                g_led.toggle();
            }
            break;
        case OperationMode::AlwaysOn:
            g_led.set(LedState::On);
            break;
        case OperationMode::AlwaysOff:
            g_led.set(LedState::Off);
            break;
    }
}
