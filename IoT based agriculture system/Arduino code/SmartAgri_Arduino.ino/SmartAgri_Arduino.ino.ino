#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 12
#define DHTTYPE DHT22
#define SOILPIN A0
#define PUMPPIN 13

DHT dht(DHTPIN, DHTTYPE);
Adafruit_PCD8544 display = Adafruit_PCD8544(6, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  dht.begin();

  display.begin();
  display.setContrast(35);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0, 0);
  display.println("Smart Farming");
  display.display();
  delay(2000);
  display.clearDisplay();

  pinMode(PUMPPIN, OUTPUT);
  digitalWrite(PUMPPIN, LOW);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int soilRaw = analogRead(SOILPIN);
  float moisture = 100 - (soilRaw / 10.23);  // Scale to 0–100%

  if (isnan(h) || isnan(t)) {
    Serial.println("Sensor error");
    return;
  }

  bool pumpControl = false;

  //  AUTOMATIC PUMP LOGIC
  if (moisture < 30) {
    pumpControl = true;   // Soil is dry → turn pump ON
  } else {
    pumpControl = false;  // Soil is wet enough → turn pump OFF
  }

  digitalWrite(PUMPPIN, pumpControl);  // Apply pump logic

  //  LCD DISPLAY
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Smart Farming");
  display.print("Temp: "); display.println(t, 1);
  display.print("Hum: "); display.println(h, 1);
  display.print("Soil: "); display.println(moisture, 0);
  if (pumpControl) display.print("Pump on");
  display.display();

  //  SEND JSON TO ESP8266
  StaticJsonDocument<200> doc;
  doc["temperature"] = t;
  doc["humidity"] = h;
  doc["moisture"] = moisture;
  doc["pump"] = pumpControl;

  serializeJson(doc, Serial);
  Serial.println();
  delay(2000);
}
