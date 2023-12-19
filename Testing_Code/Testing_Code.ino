#include <esp_now.h>
#include <ArduinoJson.h>
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 2
#define soilpin 4
const char ssid[] = "Hackathon";
const char pass[] = "";
const int water = 1159;
const int air = 3000;
const int highPin = 21;
const int lowPin = 19;
const int dhtpinhigh = 15;

void setup(){
  // WiFi.begin(ssid,pass);
  // WiFi.mode(WIFI_STA);
Serial.begin(115200);
pinMode(dhtpinhigh,OUTPUT);
  pinMode(soilpin,INPUT);
  pinMode(highPin,OUTPUT);
  pinMode(lowPin,OUTPUT);
  digitalWrite(highPin,HIGH);
  digitalWrite(lowPin,LOW);
  digitalWrite(dhtpinhigh,HIGH);
}

void loop(){
  int soilData = analogRead(soilpin);
  Serial.println(soilData);
  int soilpercetage = map(soilData,air,water, 0,100);
  Serial.print("Soil Percentage: ");
  Serial.println(soilpercetage);
  DHT.read(DHT11_PIN);
  Serial.print("temp:");
  Serial.print(DHT.temperature);
  Serial.print("  humi:");
  Serial.println(DHT.humidity);
  delay(1000);
}
