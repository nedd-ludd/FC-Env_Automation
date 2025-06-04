#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT22
#define RELAY_PIN 8

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Relay off (active-low: HIGH = off)
  Serial.println("Humidity control ready");
}

void loop() {
float humidity = dht.readHumidity();
float temperature = dht.readTemperature(); // Celsius

if (isnan(humidity) || isnan(temperature)) {
  Serial.println("Sensor read failed.");
  delay(2000);
  return;
}

Serial.print("Humidity: ");
Serial.print(humidity);
Serial.print(" %\t");

Serial.print("Temp: ");
Serial.print(temperature);
Serial.println(" °C");


  // Hysteresis logic: turn relay ON if humidity < 89, OFF if >= 97.
  if (humidity < 90) {
    digitalWrite(RELAY_PIN, LOW);  // Relay ON (test load on)
    Serial.println("Relay ON");
  }
  else if (humidity >= 97) {
    digitalWrite(RELAY_PIN, HIGH); // Relay OFF
    Serial.println("Relay OFF");
  }
  // Else maintain current state.

  delay(2000);
}
