

/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>
#include <SPI.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;



const char* ssid     = "Royal Dreams";
const char* password = "00000000";
const size_t capacity = JSON_OBJECT_SIZE(2) + 60;

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

}

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    WiFiClient client;
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;
      String url = "http://us-central1-obscure-lock.cloudfunctions.net/API/getStatus?id=1";
      http.begin(url);

      //  http.addHeader("Content-Type","text/plain");

      int httpCode = http.GET();
      Serial.print(httpCode);
      String data = http.getString();

      DynamicJsonDocument doc(capacity);

      // Parse JSON object
      DeserializationError error = deserializeJson(doc, data);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      
      //lastmessage = getlastline(data);
      Serial.print(doc["status"].as<bool>());
      Serial.print('\n');
      http.end();
    }
  } else {
    Serial.printf("[HTTP} Unable to connect\n");
  }
  delay(100);
}
