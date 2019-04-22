
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHTesp.h"

const int moisturePin = 5;  // Digital input pin that the soil moisture digital analog output is attached to
const int dh22Pin = 4;      // Digital input pin the DH22 digital output is attached to

// Blynk Auth Token 
char auth[] = "a86fa18453a3497f985fdac8268ddc8a";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "fwextern";
char pass[] = "2018FW#01";

DHTesp dht;
BlynkTimer timer;

// Called by myTimer every 10 seconds to push sensor updates to the Blynk Cloud
void myTimerEvent()
{
 
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  int soil = digitalRead(moisturePin);
  
  Blynk.virtualWrite(V5, temperature);
  Blynk.virtualWrite(V4, humidity);
  Blynk.virtualWrite(V3, soil);

  // soil warning - sensor returns '1' when soil is wet 
  // to turn Soil dry alarm on send 255. Send '0' to turn alarm off
  Blynk.virtualWrite(V2, soil==0 ? 255 : 0);
}

void setup() {
  Serial.begin(9600);

  // init DH22 module
  dht.setup(dh22Pin, DHTesp::DHT22);

  // Connect to wifi and Blynk
  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every 10 seconds
  timer.setInterval(10000L, myTimerEvent);
}

void loop() {
  // put your main code here, to run repeatedly:

  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
