#include <Arduino.h>


constexpr uint8_t FAN_PIN = 15;
constexpr uint8_t LED_GREEN = 16;
constexpr uint8_t LED_RED = 17;

constexpr uint64_t FAN_OFF_TICKS = 2000000;
constexpr uint64_t FAN_ON_TICKS = 5000000;

volatile bool g_isFanOn = false;
hw_timer_t *timer = NULL;


void IRAM_ATTR onTimer() {
  if (!g_isFanOn) {
    g_isFanOn =true;
    digitalWrite(FAN_PIN, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);

    //setting up the timer
    timerAlarmWrite(timer, FAN_ON_TICKS, true);
  } else {
    g_isFanOn = false;
    digitalWrite(FAN_PIN, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);

    //setting up the timer
    timerAlarmWrite(timer, FAN_OFF_TICKS, true);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  timer = timerBegin(0, 80, true);

  timerAttachInterrupt(timer, &onTimer, true);

  g_isFanOn = true;
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);

  timerAlarmWrite(timer, FAN_ON_TICKS, true);
  timerAlarmEnable(timer);

  Serial.println("Setup was setuped ;)");
}

void loop() {  
}