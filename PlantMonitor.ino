#include <ESP8266WiFi.h>

// Blynk IoT cloud
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>

// ESP8266 WiFi Connection Manager with fallback web configuration portal
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// DHT sensor library
#include "DHTesp.h"

// define pins
const int moistureDigitalPin = D5;  // Digital input pin that the soil moisture digital analog output is attached to
const int moistureAnaloguePin = A0; // Analogue input pin the DH22 analogue output is attached to
const int dh22Pin = D4;      // Digital input pin the DH22 digital output is attached to

// Blynk Auth Token 
// char auth[] = "your auth token here" or specify it in auth.h;
#include "auth.h"



DHTesp dht;
BlynkTimer timer;

// Called by myTimer every 10 seconds to push sensor updates to the Blynk Cloud
void myTimerEvent()
{
 
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  int soilD = digitalRead(moistureDigitalPin);
  int soilA = analogRead(moistureAnaloguePin);
  
  Blynk.virtualWrite(V5, temperature);
  Blynk.virtualWrite(V4, humidity);
  Blynk.virtualWrite(V3, soilA);

  // soil warning - sensor returns '0' when soil is wet '1' when dry
  // to turn Soil dry alarm on send 255 when wet
  Blynk.virtualWrite(V2, soilD==0 ? 0 : 255);

  Serial.print("Temp:");
  Serial.println(temperature);

  Serial.print("Humidity:");
  Serial.println(humidity);

  Serial.print("Soil mositure (analogue):");
  Serial.println(soilA);

  Serial.print("Soil mositure (digital):");
  Serial.println(soilD);  
}

void setup() {
  Serial.begin(9600);

  // init DH22 module
  dht.setup(dh22Pin, DHTesp::DHT22);

  // Connect to wifi. If unable to connect, switch to AP mode to allow user to config wifi settings
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");  

  Serial.println("Connected to wifi.");

  // Connect to wifi and Blynk
  Blynk.config(auth);

  // Setup a function to be called every 10 seconds
  timer.setInterval(10000L, myTimerEvent);
}

void loop() {
  // put your main code here, to run repeatedly:

  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
