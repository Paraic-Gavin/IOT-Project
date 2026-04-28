#include "DHT.h"

#define DHTPIN 15      // Pin where the DHT11 is connected
#define DHTTYPE DHT11 // DHT11 sensor

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("DHT11 Sensor Test Started...");
}

void loop() {
  // Wait a few seconds between measurements
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
}
