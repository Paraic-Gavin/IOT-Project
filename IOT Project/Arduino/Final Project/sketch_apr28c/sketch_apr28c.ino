#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <DHT.h>

// WIFI
const char *ssid = "Podge";
const char *password = "Bouncer1";

// PINS
#define PIR_PIN   14
#define DHTPIN    15
#define DHTTYPE   DHT11

#define RELAY_LIGHT   26   // Light
#define RELAY_BUZZER  27   // Buzzer (via relay)

// TIMER
unsigned long motionTimer = 0;
const unsigned long motionDelay = 30000;

// STATES
float temperature = 0;
float humidity = 0;
int motion = LOW;

bool lightState = false;
bool buzzerState = false;

// MANUAL CONTROL
bool manualLight = false;
bool manualBuzzer = false;

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

// WEBPAGE HTML
String homePage = F(R"=====(<!DOCTYPE html>
<html>
<head>
<meta http-equiv="refresh" content="2">
<title>Smart Classroom</title>
<style>
body { background-color: DodgerBlue; font-family: Arial; text-align: center; color: navy; }
table { margin: auto; border-collapse: collapse; background-color: white; }
th, td { padding: 10px; font-size: 20px; border: 1px solid navy; }
button { font-size: 18px; padding: 10px 20px; margin: 5px; border-radius: 10px; }
</style>
</head>

<body>
<h1>Smart Classroom Energy Saver</h1>

<table>
<tr><th>Sensor</th><th>Value</th></tr>
<tr><td>Temperature</td><td>%TEMP% Degrees</td></tr>
<tr><td>Humidity</td><td>%HUM% %</td></tr>
<tr><td>Motion</td><td>%PIR%</td></tr>
<tr><td>Light</td><td>%LIGHT%</td></tr>
<tr><td>Buzzer</td><td>%BUZZER%</td></tr>
</table>

<h2>Manual Controls</h2>

<a href="/light/on"><button>Light ON</button></a>
<a href="/light/off"><button>Light OFF</button></a>
<a href="/light/auto"><button>Light AUTO</button></a>

<br><br>

<a href="/buzzer/on"><button>Buzzer ON</button></a>
<a href="/buzzer/off"><button>Buzzer OFF</button></a>
<a href="/buzzer/auto"><button>Buzzer AUTO</button></a>

</body>
</html>
)=====");

// SYSTEM LOGIC
void updateSystem() {

  motion = digitalRead(PIR_PIN);
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // LIGHT (PIR)
  if (!manualLight) {
    if (motion == HIGH) {
      motionTimer = millis();
      digitalWrite(RELAY_LIGHT, LOW);
      lightState = true;
    } else if (millis() - motionTimer >= motionDelay) {
      digitalWrite(RELAY_LIGHT, HIGH);
      lightState = false;
    }
  }

  // BUZZER (TEMP) 
  if (!manualBuzzer) {
    if (!isnan(temperature) && temperature > 26.0) {
      digitalWrite(RELAY_BUZZER, LOW);   // ON
      buzzerState = true;
    } else {
      digitalWrite(RELAY_BUZZER, HIGH);  // OFF
      buzzerState = false;
    }
  }

  //  SERIAL OUTPUT 
  Serial.println("----");

  Serial.println(motion ? "Motion detected" : "No motion");
  Serial.println(lightState ? "Light is ON" : "Light is OFF");

  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" Degrees");
  } else {
    Serial.println("Error reading temperature");
  }

  if (!isnan(humidity)) {
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  } else {
    Serial.println("Error reading humidity");
  }

  Serial.println(buzzerState ? "Buzzer is ON" : "Buzzer is OFF");

  Serial.print("Light Mode: ");
  Serial.println(manualLight ? "MANUAL" : "AUTO");

  Serial.print("Buzzer Mode: ");
  Serial.println(manualBuzzer ? "MANUAL" : "AUTO");
}

// WEB 
void handleRoot() {
  String page = homePage;

  page.replace("%TEMP%", isnan(temperature) ? "Error" : String(temperature));
  page.replace("%HUM%", isnan(humidity) ? "Error" : String(humidity));
  page.replace("%PIR%", motion ? "Detected" : "None");
  page.replace("%LIGHT%", lightState ? "ON" : "OFF");
  page.replace("%BUZZER%", buzzerState ? "ON" : "OFF");

  server.send(200, "text/html", page);
}

// ROUTES 
void setupRoutes() {

  server.on("/", handleRoot);

  server.on("/light/on", []() {
    manualLight = true;
    digitalWrite(RELAY_LIGHT, LOW);
    lightState = true;
    server.sendHeader("Location", "/"); server.send(303);
  });

  server.on("/light/off", []() {
    manualLight = true;
    digitalWrite(RELAY_LIGHT, HIGH);
    lightState = false;
    server.sendHeader("Location", "/"); server.send(303);
  });

  server.on("/light/auto", []() {
    manualLight = false;
    updateSystem();
    server.sendHeader("Location", "/"); server.send(303);
  });

  server.on("/buzzer/on", []() {
    manualBuzzer = true;
    digitalWrite(RELAY_BUZZER, LOW);
    buzzerState = true;
    server.sendHeader("Location", "/"); server.send(303);
  });

  server.on("/buzzer/off", []() {
    manualBuzzer = true;
    digitalWrite(RELAY_BUZZER, HIGH);
    buzzerState = false;
    server.sendHeader("Location", "/"); server.send(303);
  });

  server.on("/buzzer/auto", []() {
    manualBuzzer = false;
    updateSystem();
    server.sendHeader("Location", "/"); server.send(303);
  });
}

// SETUP 
void setup() {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_BUZZER, OUTPUT);

  digitalWrite(RELAY_LIGHT, HIGH);
  digitalWrite(RELAY_BUZZER, HIGH);

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  setupRoutes();
  server.begin();

  Serial.println("System Ready");
}

// ===== LOOP =====
void loop() {
  server.handleClient();
  updateSystem();
  delay(500);
}
