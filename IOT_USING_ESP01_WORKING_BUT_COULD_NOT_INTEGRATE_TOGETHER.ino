#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "FIRE ALERT 1"
#define BLYNK_AUTH_TOKEN ""

#include <SoftwareSerial.h>
#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// WiFi credentials
char ssid[] = "S25";
char pass[] = "12345678";

// SoftwareSerial to communicate with ESP-01
SoftwareSerial EspSerial(2, 3); // RX, TX
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

bool fireDetected = false;
unsigned long previousMillis = 0;
const long interval = 500; // Check sensors every 500 ms

void checkFlame() {
  int leftValue   = analogRead(A0);
  int middleValue = analogRead(A1);
  int rightValue  = analogRead(A2);

  // Send to Blynk virtual pins
  Blynk.virtualWrite(V0, middleValue);
  Blynk.virtualWrite(V1, leftValue);
  Blynk.virtualWrite(V2, rightValue);

  // One-line serial output
  Serial.print("Left: ");
  Serial.print(leftValue);
  Serial.print(" | Middle: ");
  Serial.print(middleValue);
  Serial.print(" | Right: ");
  Serial.println(rightValue);

  // Fire detection logic
  if (leftValue < 1000 || middleValue < 1000 || rightValue < 1000) {
    if (!fireDetected) {
      Blynk.logEvent("fire_alert", "🔥 Fire Detected!");
      Serial.println("🔥 Fire Detected! Notification sent.");
      fireDetected = true;
    }
  } else {
    if (fireDetected) {
      Serial.println("✅ Fire cleared.");
    }
    fireDetected = false;
  }
}

void setup() {
  Serial.begin(9600);
  EspSerial.begin(ESP8266_BAUD);
  delay(1000); // Allow ESP to initialize

  Blynk.begin(BLYNK_AUTH_TOKEN, wifi, ssid, pass);
  Blynk.logEvent("fire_alert", "🚨 System Powered ON!");
  Serial.println("🚨 Boot notification sent.");
}

void loop() {
  Blynk.run();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    checkFlame();
  }
}
