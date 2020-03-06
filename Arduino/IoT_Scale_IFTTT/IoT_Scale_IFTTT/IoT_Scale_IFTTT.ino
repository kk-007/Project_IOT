#include "HX711.h"
#include <ESP8266WiFi.h>

char ssid[] = "Royal Dream";       // your network SSID (name)
char password[] = "000000008";  // your network key
HX711 cell(D2, D3); // The two pins connected to the Load Cell and Node MCU 

void setup() {
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


  
}
long valCali = 0;
long val = 0;
float count = 0;


void loop() {
  
  IPAddress ip = WiFi.localIP();

  // Calibrating the Load Cell
  
  count = count + 1;
  
  val = cell.read(); // most recent reading
  Serial.println(val);
  valCali = (((val-8279253)/22872.0)*50); 
  //Serial.println(valCali);
  
  // Conditions for sending a notification to mobile phone, current definition is acheived if the weight is over
  // 400g, the delay of 10000 sends another notification every 10 seconds

  if( valCali > 0){

    Serial.println("Weight Over 100");
    delay(10000); // Change this value to delay notifications being sent
  }
   else{ 
  }
  
}
