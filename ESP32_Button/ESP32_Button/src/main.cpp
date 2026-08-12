#include <Arduino.h>

#define EXT_BUTTON 18 //external button
#define BOOT_BUTTON 0 //boot button
#define LED_RED   15
#define LED_GREEN 16

uint32_t currentBlinkSpeed = 400; //delay for LED blinking

void LedBlinking(uint32_t speedMs); //function for LED blinking


void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  pinMode(EXT_BUTTON, INPUT_PULLDOWN);
  pinMode(BOOT_BUTTON, INPUT_PULLUP);
}


void loop() {
  //read the button's state
  int currentExtState = digitalRead(EXT_BUTTON);
  int currentBootState = digitalRead(BOOT_BUTTON);


  //speed up the led blinking
  if (currentExtState == HIGH) {
    if (currentBlinkSpeed >= 100) {
      currentBlinkSpeed -= 50; //decreasing delay for faster blinking
      Serial.printf("SPEED UP: current speed - %d\n", currentBlinkSpeed);
      delay(50);        //delay to avoid debounce
    }
  }  
  
  //slowing down the led blinking
  if (currentBootState == LOW) {
    if (currentBlinkSpeed <= 1000) {
      currentBlinkSpeed += 50; //increasing delay for slower blinking
      Serial.printf("SLOW DOWN: current speed - %d\n", currentBlinkSpeed);
      delay(50);        //delay to avoid debounce
    }
  }


  //the main logic
  LedBlinking(currentBlinkSpeed);
}



void LedBlinking(uint32_t speedMs) {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  delay(speedMs);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  delay(speedMs);
}
