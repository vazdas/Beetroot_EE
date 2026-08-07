#include <Arduino.h>

#define LED_RED 15
#define LED_BLUE 9

int speed = 200;

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