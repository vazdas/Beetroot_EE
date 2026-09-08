#ifndef TRAFFIC_LIGHT_FSM_H
#define TRAFFIC_LIGHT_FSM_H

#include <Arduino.h>

typedef enum {
    LIGHT_STATE_GO = 0,
    LIGHT_STATE_GREEN_BLINK,
    LIGHT_STATE_WARNING,
    LIGHT_STATE_STOP,
    LIGHT_STATE_READY
} LightState_t;

typedef struct {
    uint8_t pinRed;
    uint8_t pinGreen;
    uint8_t pinYellow;
    uint8_t blinkCount;
    LightState_t state;
    uint32_t lastChangeTime;
    bool pedestrianWaiting;
} TrafficLight_FSM_t;

int TrafficLight_Init(TrafficLight_FSM_t *fsm, uint8_t pinRed, uint8_t pinGreen, uint8_t pinYellow);
int TrafficLight_Update(TrafficLight_FSM_t *fsm);

#endif // TRAFFIC_LIGHT_FSM_H