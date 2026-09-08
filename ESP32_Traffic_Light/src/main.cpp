#include <Arduino.h>
#include "trafficLightFSM.h"
#include "buttonFSM.h"

#define RED_LED_PIN    15
#define YELLOW_LED_PIN 16
#define GREEN_LED_PIN  17
#define TRAFFIC_LIGHT_BTN 18

// creating objects for traffic light and button
static TrafficLight_FSM_t myTrafficLight;
static Button_FSM_t trafficLightButton;

// creating callback for button
void onPedestrianButtonPress(void* arg) {
    Serial.println("Pedestrian pressed the button!");
    myTrafficLight.pedestrianWaiting = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(TRAFFIC_LIGHT_BTN, INPUT_PULLUP);
    
    if (TrafficLight_Init(&myTrafficLight, RED_LED_PIN, GREEN_LED_PIN, YELLOW_LED_PIN) != 0) {
        Serial.println("Error: initialization problem!!");
    }

    Button_FSM_Init(&trafficLightButton, TRAFFIC_LIGHT_BTN, 50, onPedestrianButtonPress, NULL, NULL);
}

void loop() {
    TrafficLight_Update(&myTrafficLight);
    Button_FSM_Update(&trafficLightButton);
}