// This code is derived from the HelloServer Example

// in the (ESP32) WebServer library .

//

// It hosts a webpage which has one temperature reading to display.

// The webpage is always the same apart from the reading which would change.

// The getTemp() function simulates getting a temperature reading.

// homePage.h contains 2 constant string literals which is the two parts of the

// webpage that never change.

// handleRoot() builds up the webpage by adding as a C++ String:

// homePagePart1 + getTemp() +homePagePart2

// It then serves the webpage with the command:  

// server.send(200, "text/html", message);

// Note the text is served as html.

//

// Replace the code in the homepage.h file with your own website HTML code.

//

// This example requires only an ESP32 and download cable. No other hardware is reuired.

// A wifi SSID and password is required.

// Written by: Natasha Rohan  12/3/23

//

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "homepage.h"

#include "DHT.h"

// ===== WIFI =====
const char *ssid = "Podge";
const char *password = "Bouncer1";

// ===== SENSOR PINS =====
#define DHTPIN 15
#define DHTTYPE DHT11
#define PIR_PIN 14

DHT dht(DHTPIN, DHTTYPE);

// ===== WEB SERVER =====
WebServer server(80);

// ===== SENSOR FUNCTIONS =====
String getTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) return "Error";
  return String(t);
}

String getHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) return "Error";
  return String(h);
}

String getMotion() {
  int motion = digitalRead(PIR_PIN);
  return motion == HIGH ? "Motion Detected" : "No Motion";
}

// ===== ROUTES =====
void handleRoot() {
  String page = homePage;   // fresh copy every request

  page.replace("%TEMP%", getTemperature());
  page.replace("%HUM%", getHumidity());
  page.replace("%PIR%", getMotion());

  server.send(200, "text/html", page);
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

// ===== SETUP =====
void setup(void) {
  Serial.begin(115200);

  pinMode(PIR_PIN, INPUT);
  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("HTTP server started");
}

// ===== LOOP =====
void loop(void) {
  server.handleClient();
  delay(2);
}
