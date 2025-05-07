#define RELAY_PIN 27 // Relay IN connected to ESP32 D27

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Relay OFF (active-low)
  Serial.println("Relay Pump Test Started");
}

void loop() {
  // Turn pump ON
  digitalWrite(RELAY_PIN, LOW); // Relay ON (active-low)
  Serial.println("Pump ON");
  delay(5000); // Run for 5 seconds
  
  // Turn pump OFF
  digitalWrite(RELAY_PIN, HIGH); // Relay OFF
  Serial.println("Pump OFF");
  delay(5000); // Off for 5 seconds
}