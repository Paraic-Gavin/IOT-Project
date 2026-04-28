#define PIR_PIN 14 

void setup() {
  Serial.begin(9600);
  pinMode(PIR_PIN, INPUT);
  Serial.println("PIR Sensor Test Started");
}

void loop() {
  int motion = digitalRead(PIR_PIN);

  if (motion == HIGH) {
    Serial.println("Motion Detected!");
  } else {
    Serial.println("No Motion.");
  }

  delay(500); // Update twice per second
}
