#include "trafficLightFSM.h"

int TrafficLight_Init(TrafficLight_FSM_t *fsm, uint8_t pinRed, uint8_t pinGreen, uint8_t pinYellow) {
    if (fsm == NULL) {
        printf("Error: FSM pointer is NULL");
        return -1;
    }

    fsm->pinRed = pinRed;
    fsm->pinGreen = pinGreen;
    fsm->pinYellow = pinYellow;
    fsm->state = LIGHT_STATE_GO;
    fsm->lastChangeTime = 0;
    fsm->blinkCount = 0;
    fsm->pedestrianWaiting = false;

    pinMode(fsm->pinRed, OUTPUT);
    pinMode(fsm->pinGreen, OUTPUT);
    pinMode(fsm->pinYellow, OUTPUT);

    digitalWrite(fsm->pinGreen, HIGH);
    digitalWrite(fsm->pinYellow, LOW);
    digitalWrite(fsm->pinRed, LOW);
    
    return 0;
}

int TrafficLight_Update(TrafficLight_FSM_t *fsm) {
    if (fsm == NULL) {
        printf("Error: FSM pointer is NULL");
        return -1;
    }
    
    uint32_t current_time = millis();

    switch (fsm->state) {
        case LIGHT_STATE_GO:
            if (current_time - fsm->lastChangeTime >= 5000 || fsm->pedestrianWaiting == true) {
                fsm->lastChangeTime = current_time;
                fsm->blinkCount = 0;
                
                fsm->pedestrianWaiting = false;                
                
                fsm->state = LIGHT_STATE_GREEN_BLINK;
            }
            break;

        case LIGHT_STATE_GREEN_BLINK:            
            if (current_time - fsm->lastChangeTime >= 500) {
                fsm->lastChangeTime = current_time;            
                
                digitalWrite(fsm->pinGreen, !digitalRead(fsm->pinGreen));            
                
                fsm->blinkCount++;            
                
                if (fsm->blinkCount >= 6) {                    
                    digitalWrite(fsm->pinGreen, LOW);                
                    
                    digitalWrite(fsm->pinYellow, HIGH);                
                    
                    fsm->state = LIGHT_STATE_WARNING;
                }
            }
            break;

        case LIGHT_STATE_WARNING:            
            if (current_time - fsm->lastChangeTime >= 2000) {
                fsm->lastChangeTime = current_time;                
                
                digitalWrite(fsm->pinYellow, LOW);
                digitalWrite(fsm->pinRed, HIGH);
                
                fsm->state = LIGHT_STATE_STOP;
            }
            break;

        case LIGHT_STATE_STOP:            
            if (current_time - fsm->lastChangeTime >= 5000) {
                fsm->lastChangeTime = current_time;                
                
                digitalWrite(fsm->pinYellow, HIGH);
                
                fsm->state = LIGHT_STATE_READY;
            }
            break;

        case LIGHT_STATE_READY:            
            if (current_time - fsm->lastChangeTime >= 2000) {
                fsm->lastChangeTime = current_time;                
                
                digitalWrite(fsm->pinRed, LOW);
                digitalWrite(fsm->pinYellow, LOW);
                digitalWrite(fsm->pinGreen, HIGH);
                
                fsm->state = LIGHT_STATE_GO;
            }
            break;
    }
    return 0;
}
