#include "HX711.h"
#include <ESP8266WiFi.h>
HX711 cell(D2, D3);

char ssid[] = "Royal Dream";       // your network SSID (name)
char password[] = "000000008";  // your network key

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

  cell.set_scale();
  cell.tare();

  long zero_factor = cell.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);

}
long valCali = 0;
long val = 0;
float count = 0;
float units;
float ounces;
float calibration_factor = 48100;
void loop() {

  // Calibrating the Load Cell
  cell.set_scale(calibration_factor);
  count = count + 1;
  units = cell.get_units(), 10;
  if (units < 0)
  {
    units = 0.00;
  }
  ounces = units * 0.035274;
  Serial.print(cell.read());
  Serial.print(" kg");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  
  Serial.println();
  Serial.println(cell.get_units());
  delay(500);
  //val = cell.read(); // most recent reading
  /*if (cell.is_ready()) {
    Serial.print("\t Units ");
    Serial.print(cell.get_units() * 10000);
    Serial.print("\t Read ");
    Serial.println(cell.read());
  }*/
  delay(1000);
}
