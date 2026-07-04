
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Hallo vom ESP32 / Wemos D1 R32 - mclab24!");
}

void loop() {
  Serial.println("Läuft...");
  delay(2000);
}
