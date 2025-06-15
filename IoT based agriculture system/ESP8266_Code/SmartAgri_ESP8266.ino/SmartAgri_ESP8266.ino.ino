#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "realme";
const char* password = "12345678";
const char* server = "http://192.168.24.15:5000/api/data";

void setup() {
  Serial.begin(9600); // Connects to Arduino
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(1000);
}

void loop() {
  if (Serial.available()) {
    String jsonData = Serial.readStringUntil('\n');

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(server);
      http.addHeader("Content-Type", "application/json");
      int httpCode = http.POST(jsonData);
      http.end();
    }
  }
}

