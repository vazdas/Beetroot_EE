#include <Arduino.h>

//The main struct for led
typedef struct {
    uint8_t pin;
    uint32_t blinkInterval;
    uint32_t lastToggleTime;
    uint8_t state;
} LedTask_t;

//Create three objects
static LedTask_t led1 = {15, 200, 0, LOW};
static LedTask_t led2 = {16, 500, 0, LOW};
static LedTask_t led3 = {17, 1000, 0, LOW};

//Initialization function
void LedTask_Init(LedTask_t *led) {
    pinMode(led->pin, OUTPUT);
    digitalWrite(led->pin, led->state);
}

//The main program for led
void LedTask_Update(LedTask_t *led) {
    uint32_t currentMillis = millis();
    
    //Checking whether the current time minus the last switching time is GREATER than the interval
    if (currentMillis - led->lastToggleTime >= led->blinkInterval) {
        //Time updating
        led->lastToggleTime = currentMillis;
        //Changing the state
        led->state = (led->state == LOW) ? HIGH : LOW;
        digitalWrite(led->pin, led->state);
    }
}

void setup() {
    //Initialize all leds
    LedTask_Init(&led1);
    LedTask_Init(&led2);
    LedTask_Init(&led3);
}

void loop() {
    //Superloop
    LedTask_Update(&led1);
    LedTask_Update(&led2);
    LedTask_Update(&led3);
}