#define BLYNK_TEMPLATE_ID "TMPL6jCfb70qv"
#define BLYNK_TEMPLATE_NAME "Watering Pump with IoT"
#define BLYNK_AUTH_TOKEN "jT1w3YGQBe9JL7hA5vUEoRBhGVgG0c3c"
#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal.h>

// Blynk credentials
char auth[] = "jT1w3YGQBe9JL7hA5vUEoRBhGVgG0c3c";
char ssid[] = "Cloud";
char pass[] = "clouderror404";

// Pin definitions
#define RELAY_PIN 27 // Relay IN on D27

// LCD pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(13, 12, 14, 15, 26, 25);

// Variables
bool pumpState = false; // Pump state (false = OFF, true = ON)

// Blynk virtual pin V1 (switch button)
BLYNK_WRITE(V1) {
  pumpState = param.asInt(); // Get switch state (0 = OFF, 1 = ON)
  digitalWrite(RELAY_PIN, pumpState ? LOW : HIGH); // Active-low
  // Update LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pump: ");
  lcd.print(pumpState ? "ON" : "OFF");
  Serial.print("Pump: ");
  Serial.println(pumpState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Relay OFF (active-low)

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Smart Moss Wall");
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");
  delay(2000);

  // Connect to Wi-Fi and Blynk
  Blynk.begin(auth, ssid, pass);

  // Clear LCD and show initial state
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pump: OFF");
  Serial.println("System Initialized");
}

void loop() {
  Blynk.run(); // Handle Blynk communication
}