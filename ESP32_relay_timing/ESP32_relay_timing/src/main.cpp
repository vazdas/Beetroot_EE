#include <Arduino.h>

struct Config {
    static constexpr uint8_t RELAY_CTRL_PIN  = 15;
    static constexpr uint8_t RELAY_SENSE_PIN = 16;
    static constexpr uint8_t TOTAL_TESTS     = 10;
    static constexpr uint32_t SETTLE_MS      = 500;
};

volatile uint32_t g_contactTimeMicros = 0;
volatile bool g_contactTriggered = false;

//Debounce in ISR
void IRAM_ATTR relayIsr() {
    if (!g_contactTriggered) {
        g_contactTimeMicros = micros();
        g_contactTriggered = true; 
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    pinMode(Config::RELAY_CTRL_PIN, OUTPUT);
    digitalWrite(Config::RELAY_CTRL_PIN, HIGH);

    pinMode(Config::RELAY_SENSE_PIN, INPUT_PULLUP);

    Serial.println("  №  |      ON delay (ms) |           OFF delay (ms)");
    Serial.println("-------------------------------------------------------");
}

void loop() {
    static uint8_t testCount = 0;
    static uint32_t onDelays[Config::TOTAL_TESTS];
    static uint32_t offDelays[Config::TOTAL_TESTS];

    if (testCount < Config::TOTAL_TESTS) {
        delay(Config::SETTLE_MS);

        //MEASUREMENT OF SWITCH-ON TIME
        attachInterrupt(digitalPinToInterrupt(Config::RELAY_SENSE_PIN), relayIsr, FALLING);
        g_contactTriggered = false;

        const uint32_t startOnMicros = micros();
        digitalWrite(Config::RELAY_CTRL_PIN, LOW); //Sending thye signal to turn On

        uint32_t timeoutMs = millis();
        while (!g_contactTriggered && (millis() - timeoutMs < 500)) {}

        const uint32_t onDelayMicros = g_contactTimeMicros - startOnMicros;
        onDelays[testCount] = onDelayMicros;

        delay(Config::SETTLE_MS);

        //MEASUREMENT OF SWITCH-OFF TIME
        attachInterrupt(digitalPinToInterrupt(Config::RELAY_SENSE_PIN), relayIsr, RISING);
        g_contactTriggered = false;

        const uint32_t startOffMicros = micros();
        digitalWrite(Config::RELAY_CTRL_PIN, HIGH); //Sending the signal to turn Off

        timeoutMs = millis();
        while (!g_contactTriggered && (millis() - timeoutMs < 500)) {}

        const uint32_t offDelayMicros = g_contactTimeMicros - startOffMicros;
        offDelays[testCount] = offDelayMicros;

        detachInterrupt(digitalPinToInterrupt(Config::RELAY_SENSE_PIN));

        //Output the current values
        Serial.printf(" %2d  | %18.2f | %17.2f\n", testCount + 1, onDelayMicros / 1000.0f, offDelayMicros / 1000.0f);

        testCount++;

        //CALCULATION OF THE AVERAGE VALUE
        if (testCount == Config::TOTAL_TESTS) {
            uint64_t sumOn = 0, sumOff = 0;
            for (uint8_t i = 0; i < Config::TOTAL_TESTS; ++i) {
                sumOn += onDelays[i];
                sumOff += offDelays[i];
            }

            const float avgOnMs = (float)sumOn / Config::TOTAL_TESTS / 1000.0f;
            const float avgOffMs = (float)sumOff / Config::TOTAL_TESTS / 1000.0f;

            Serial.println("-------------------------------------------------------");
            Serial.printf("AVERAGE: %14.2f ms | %17.2f ms\n", avgOnMs, avgOffMs);
            Serial.println("=======================================================");
        }
    }
}
