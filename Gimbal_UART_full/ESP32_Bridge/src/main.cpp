#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "secrets.h"

// --- ТВОЇ НАЛАШТУВАННЯ WI-FI ---
const char* ssid = SECRET_WIFI_SSID;      
const char* password = SECRET_WIFI_PASS;

// Створюємо веб-сервер на стандартному порту 80
WebServer server(80);

// Піни для UART2 (зв'язок з STM32)
#define RXD2 16
#define TXD2 17

// Змінна для зберігання останнього кута
String latest_angle = "Чекаю дані...";

// --- HTML СТОРІНКА (Зберігається в пам'яті ESP32) ---
const char* html_page = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Gimbal Telemetry</title>
  <style>
    body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #1e1e1e; color: #fff; text-align: center; margin-top: 10vh; }
    h1 { color: #00adb5; font-size: 2.5em; }
    p { color: #aaaaaa; font-size: 1.2em; }
    .angle-box { font-size: 5em; font-weight: bold; background: #2d2d2d; display: inline-block; padding: 20px 60px; border-radius: 15px; border: 3px solid #00adb5; box-shadow: 0 0 20px rgba(0, 173, 181, 0.4); margin-top: 20px; }
  </style>
  <script>
    // Цей скрипт працює в твоєму браузері і кожні 200 мс просить у ESP32 новий кут
    setInterval(function() {
      fetch('/angle').then(response => response.text()).then(text => {
        document.getElementById("angle_val").innerHTML = text;
      });
    }, 200);
  </script>
</head>
<body>
  <h1>STM32 Gimbal Telemetry 🚀</h1>
  <p>Live stream via UART-to-WiFi Bridge</p>
  <div class="angle-box" id="angle_val">...</div>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  // 1. Підключення до Wi-Fi
  Serial.println();
  Serial.print("Підключення до Wi-Fi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  // Чекаємо, поки роутер видасть нам IP-адресу
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\n✅ Wi-Fi підключено!");
  Serial.print("🌐 ВІДКРИЙ В БРАУЗЕРІ: http://");
  Serial.println(WiFi.localIP());

  // 2. Налаштування маршрутів сервера
  // Що віддавати, коли заходимо на головну сторінку
  server.on("/", []() {
    server.send(200, "text/html", html_page);
  });

  // Що віддавати, коли JavaScript скрипт просить тільки цифри
  server.on("/angle", []() {
    server.send(200, "text/plain", latest_angle);
  });

  // Запуск сервера
  server.begin();
}

void loop() {
  // Дозволяємо серверу обробляти запити від браузера
  server.handleClient();

  // Читаємо дані від STM32 (якщо вони є)
  if (Serial2.available()) {
    String incoming = Serial2.readStringUntil('\n');
    incoming.trim(); // Прибираємо невидимі символи переносу рядка
    
    if (incoming.length() > 0) {
      latest_angle = incoming; // Оновлюємо змінну для сайту
    }
  }
}
