#include <Arduino.h>

// Піни для з'єднання з STM32
// RX2 (GPIO 16) - сюди втикаємо дріт від STM32 TX (PA9)
// TX2 (GPIO 17) - сюди втикаємо дріт від STM32 RX (PA10)
#define RXD2 16
#define TXD2 17

void setup() {
  // 1. Запускаємо USB-порт (швидкість 115200)
  // Щоб ти бачив повідомлення на комп'ютері
  Serial.begin(115200);
  
  // 2. Запускаємо порт для спілкування з STM32
  // Швидкість теж 115200 (має співпадати з налаштуваннями в CubeMX!)
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  Serial.println("ESP32: Я готова! Слухаю STM32...");
}

void loop() {
  // Перевіряємо, чи прийшло щось від STM32
  if (Serial2.available()) {
    // Читаємо весь рядок до символу \n (Enter)
    String message = Serial2.readStringUntil('\n');
    
    // Виводимо в консоль комп'ютера
    Serial.print("Отримано від STM32: ");
    Serial.println(message);
  }
}