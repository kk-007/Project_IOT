

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

#include <Servo.h>

Servo myservo;

ESP8266WiFiMulti WiFiMulti;
bool previous;
const char *ssid = "Royal Dreams";
const char *password = "00000000";
const size_t capacity = JSON_OBJECT_SIZE(2) + 60;

void setup()
{

    Serial.begin(115200);
    // Serial.setDebugOutput(true);
    myservo.attach(2);//D4
    myservo.write(0);
    previous = false;
    Serial.println();

    for (uint8_t t = 4; t > 0; t--)
    {
        Serial.printf("[SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }

    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP(ssid, password);
}

void loop()
{
    // wait for WiFi connection
    if ((WiFiMulti.run() == WL_CONNECTED))
    {

        WiFiClient client;
        if (WiFi.status() == WL_CONNECTED)
        {
            HTTPClient http;
            String url = "http://us-central1-obscure-lock.cloudfunctions.net/API/getStatus?id=1";
            http.begin(url);

            //  http.addHeader("Content-Type","text/plain");

            int httpCode = http.GET();
            Serial.println(httpCode);
            String data = http.getString();

            DynamicJsonDocument doc(capacity);

            // Parse JSON object
            DeserializationError error = deserializeJson(doc, data);
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                return;
            }
            else
            {

                //lastmessage = getlastline(data);
                bool current = doc["status"].as<bool>();
                Serial.println(doc["status"].as<bool>());
                if (current != previous)
                {
                    if (current)
                    {
                        openDoor();
                    }
                    else
                    {
                        closeDoor();
                    }
                }
                // Serial.print('\n');
                previous = doc["status"].as<bool>();
            }

            http.end();
        }
    }
    else
    {
        Serial.printf("[HTTP} Unable to connect\n");
    }
    delay(100);
}
void openDoor()
{
    int pos;
    for (pos = 0; pos <= 90; pos += 1)
    { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos); // tell servo to go to position in variable 'pos'
        delay(15);
        Serial.println(pos); // waits 15ms for the servo to reach the position
    }
}
void closeDoor()
{
    int pos;
    for (pos = 90; pos >= 0; pos -= 1)
    {                       // goes from 180 degrees to 0 degrees
        myservo.write(pos); // tell servo to go to position in variable 'pos'
        delay(15);
        Serial.println(pos); // waits 15ms for the servo to reach the position
    }
}
