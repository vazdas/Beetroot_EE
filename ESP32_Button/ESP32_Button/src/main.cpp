#include <Arduino.h>

#define BUTTON_IN 16
#define LED_OUT   15

int lastButtonState = LOW;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_OUT, OUTPUT);
  pinMode(BUTTON_IN, INPUT_PULLDOWN);
  digitalWrite(LED_OUT, LOW);
}

void loop() {
  int currentState = digitalRead(BUTTON_IN);

  //checking whether the status was changed
  if (currentState != lastButtonState) {
    if (currentState == HIGH) {
      digitalWrite(LED_OUT, HIGH);
      Serial.println("Button pressed");
    } else {
      digitalWrite(LED_OUT, LOW);
      Serial.println("Button released");
    }

    delay(100);

    //updateing current status
    lastButtonState = currentState;

  }
}
