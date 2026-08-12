#include <Arduino.h>

#define LED_RED 16
#define LED_BLUE 15

uint32_t speed = 1000; //the speed of the police light

void PoliceLight();

void setup() {
  Serial.begin(115200);
  delay(2000);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
}

void loop() {
  PoliceLight();
}

void PoliceLight(){
  digitalWrite(LED_BLUE,HIGH);
  digitalWrite(LED_RED,LOW);
  delay(speed);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_RED,HIGH);
  delay(speed);
}
