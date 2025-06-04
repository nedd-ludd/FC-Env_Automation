#include <DHT.h>

// SENSOR PINS 23-53(ODD)

// SENSOR PINS 22-52(EVEN)
#define DHTPIN 22
#define DHTTYPE DHT22

// RELAY PINS 2-8
#define HUM_RELAY_PIN 2
#define COOLING_RELAY_PIN 3
#define HEATING_RELAY_PIN 4

// 5 inc 2 heater
// 6 inc 1 heater
// 7 main fan
// 8 lighting

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  //
  pinMode(HUM_RELAY_PIN, OUTPUT);
  pinMode(COOLING_RELAY_PIN, OUTPUT);
  pinMode(HEATING_RELAY_PIN, OUTPUT);
  //
  digitalWrite(COOLING_RELAY_PIN, HIGH); // OFF
  digitalWrite(HEATING_RELAY_PIN, HIGH); // OFF
  digitalWrite(HUM_RELAY_PIN, HIGH); // Relay off (active-low: HIGH = off)
  //
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
    digitalWrite(HUM_RELAY_PIN, LOW);  // Relay ON (test load on)
    Serial.println("Relay ON");
  }
  else if (humidity >= 97) {
    digitalWrite(HUM_RELAY_PIN, HIGH); // Relay OFF
    Serial.println("Relay OFF");
  }

  // Temperature control
if (temperature > 24.5) {
  digitalWrite(COOLING_RELAY_PIN, LOW);  // Cooling ON
  digitalWrite(HEATING_RELAY_PIN, HIGH); // Heating OFF
  Serial.println("Cooling ON");
}
else if (temperature < 21.5) {
  digitalWrite(COOLING_RELAY_PIN, HIGH);  // Cooling OFF
  digitalWrite(HEATING_RELAY_PIN, LOW);   // Heating ON
  Serial.println("Heating ON");
}
else {
  digitalWrite(COOLING_RELAY_PIN, HIGH); // Both OFF
  digitalWrite(HEATING_RELAY_PIN, HIGH);
  Serial.println("Temp in range. Both OFF");
}

  // Else maintain current state.

  delay(2000);
}
