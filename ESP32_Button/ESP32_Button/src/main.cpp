#include <Arduino.h>

#define EXT_BUTTON 18 //external button
#define BOOT_BUTTON 0 //boot button
#define LED_RED   15
#define LED_GREEN 16

uint32_t blinkDelay = 400; //delay for LED blinking

void LedBlinking(uint32_t delayMs); //function for LED blinking



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


  //below blocks compare the state of the buttons
  if (currentExtState == HIGH && currentBootState == LOW) {
    blinkDelay = 400; //delay for normal blinking
    Serial.println("Blinking mode: NORMAL");
    delay(50);        //delay to avoid debounce
  }


  if (currentExtState == HIGH && currentBootState == HIGH) {
    blinkDelay = 50; //delay for fast blinking
    Serial.println("Blinking mode: FAST");
    delay(50);        //delay to avoid debounce
  }
  
  
  if (currentBootState == LOW && currentExtState == LOW) {
    blinkDelay = 700; //delay for slow blinking
    Serial.println("Blinking mode: SLOW");
    delay(50);        //delay to avoid debounce
  }

  //the main logic
  LedBlinking(blinkDelay);
}



void LedBlinking(uint32_t delayMs) {
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  delay(delayMs);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);
  delay(delayMs);
}
