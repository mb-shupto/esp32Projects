#define BLYNK_TEMPLATE_ID "TMPL6FLMDJTk8"
#define BLYNK_TEMPLATE_NAME "Plant watering with IoT"
#define BLYNK_AUTH_TOKEN "ij2lGkA6uKE9TysMwsgGbnQTKMmBzh7s"

#define BLYNK_FIRMWARE_VERSION "0.1.0"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <BH1750.h>
#include <MQ135.h>

// Blynk credentials
char auth[] = "ij2lGkA6uKE9TysMwsgGbnQTKMmBzh7s";
char ssid[] = "Cloud";
char pass[] = "clouderror404";

// Pin definitions
#define SensorPin 34       // Soil moisture AOUT
#define MQ135_PIN 35       // MQ-135 analog
#define DHT_PIN 4          // DHT22 data
#define RELAY_PIN 27       // Relay IN
#define DHTTYPE DHT22      // DHT22 type

// Soil moisture calibration
const int dryValue = 2834; // Dry soil
const int wetValue = 2250; // Wet soil

// Thresholds
#define MOISTURE_THRESHOLD_LOW 10   // Pump ON
#define MOISTURE_THRESHOLD_HIGH 70  // Pump OFF

// Initialize sensors
DHT dht(DHT_PIN, DHTTYPE);
BH1750 lightMeter;
MQ135 gasSensor(MQ135_PIN);

// Variables
const int numReadings = 5; // For smoothing
int moistureReadings[numReadings]; // Array for readings
int readingIndex = 0; // Current reading index
long totalMoisture = 0; // Sum of readings
bool pumpState = false; // Pump state

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Relay OFF (active-low)

  // Initialize smoothing array
  for (int i = 0; i < numReadings; i++) {
    moistureReadings[i] = 0;
  }

  // Initialize sensors
  dht.begin();
  Wire.begin();
  lightMeter.begin();

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);

  Serial.println("System Initialized");
}

void loop() {
  Blynk.run(); // Handle Blynk

  // Read sensors
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  float lightLevel = lightMeter.readLightLevel();
  float airQuality = gasSensor.getPPM();

  // Read and smooth soil moisture
  totalMoisture = totalMoisture - moistureReadings[readingIndex];
  moistureReadings[readingIndex] = analogRead(SensorPin);
  totalMoisture = totalMoisture + moistureReadings[readingIndex];
  readingIndex = (readingIndex + 1) % numReadings;
  int avgMoisture = totalMoisture / numReadings;

  // Map to percentage
  float moisturePercent = map(avgMoisture, dryValue, wetValue, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  // Automatic pump control
  if (moisturePercent < MOISTURE_THRESHOLD_LOW) {
    digitalWrite(RELAY_PIN, LOW); // Relay ON (active-low)
    if (!pumpState) {
      pumpState = true;
      Serial.println("Pump ON (Auto)");
    }
  } else if (moisturePercent > MOISTURE_THRESHOLD_HIGH) {
    digitalWrite(RELAY_PIN, HIGH); // Relay OFF
    if (pumpState) {
      pumpState = false;
      Serial.println("Pump OFF (Auto)");
    }
  }

  // Send to Blynk
  Blynk.virtualWrite(V2, moisturePercent);
  Blynk.virtualWrite(V3, temperature);
  Blynk.virtualWrite(V4, humidity);
  Blynk.virtualWrite(V5, lightLevel);
  Blynk.virtualWrite(V6, airQuality);

  // Serial output
  Serial.print("Moisture Raw: "); Serial.print(avgMoisture); Serial.print(" -> ");
  Serial.print(moisturePercent); Serial.println("%");
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println("°C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println("%");
  Serial.print("Light: "); Serial.print(lightLevel); Serial.println(" lux");
  Serial.print("Air Quality: "); Serial.print(airQuality); Serial.println(" ppm");
  Serial.println("-------------------");

  delay(2000); // Update every 2 seconds
}