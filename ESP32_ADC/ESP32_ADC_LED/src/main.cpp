#include <Arduino.h>

// Використовуємо пін ADC1 (наприклад, GPIO1)
#define LDR_PIN 1 

const float ADC_MAX = 4095.0;     // 12 біт = 2^12 - 1
const float U_REF_MV = 3100.0;    // 3.1 В = 3100 мВ

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Встановлюємо 12-бітну роздільну здатність АЦП (0..4095)
  analogReadResolution(12);

  // Красивий заголовок таблиці в серійну консоль
  Serial.println("\n=============================================================");
  Serial.println("RAW\t| U_calc (mV)\t| U_meas (mV)\t| Error (%)");
  Serial.println("=============================================================");
}

void loop() {
  // 1. Зчитування сирих даних (RAW)
  int raw = analogRead(LDR_PIN);

  // 2. Обчислення напруги за формулою зі знімка
  float u_calc = ((float)raw / ADC_MAX) * U_REF_MV;

  // 3. Зчитування напруги через вбудований заводський калібрувальник ESP32
  uint32_t u_measured = analogReadMilliVolts(LDR_PIN);

  // 4. Розрахунок відносної похибки
  float error_pct = 0.0;
  if (u_measured > 0) {
    error_pct = (abs(u_calc - u_measured) / (float)u_measured) * 100.0;
  }

  // 5. Вивід структурованої таблиці
  Serial.printf("%d\t| %.2f\t\t| %d\t\t| %.2f%%\n", raw, u_calc, u_measured, error_pct);

  // Затримка 100 мс за вимогою завдання
  delay(100); 
}