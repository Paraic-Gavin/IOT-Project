

#include <DHT.h>


#define PIR_PIN   14
#define DHTPIN    15
#define DHTTYPE   DHT11


// Relays are ACTIVE LOW
#define RELAY_1   26   // Motion → LED via relay
#define RELAY_2   5   // Temperature → LED via relay


DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);

  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);


  // Turn all relays OFF
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);


  dht.begin();

  Serial.println("System Ready");
}

void loop() {

  // PIR MOTION
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    digitalWrite(RELAY_1, LOW);   // Relay ON → LED ON
    Serial.println("Motion detected");
  } else {
    digitalWrite(RELAY_1, HIGH);  // Relay OFF → LED OFF
    Serial.println("No motion");
  }

  // TEMPERATURE
  float temperature = dht.readTemperature();

  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    if (temperature < 20.0) {
      digitalWrite(RELAY_2, LOW);   // Relay ON → LED ON
      Serial.println("Temp < 20°C");
    } else {
      digitalWrite(RELAY_2, HIGH);  // Relay OFF → LED OFF
      Serial.println("Temp ≥ 20°C");
    }

  } else {
    Serial.println("Error reading DHT11");
  }



  delay(2500); // DHT11 
}