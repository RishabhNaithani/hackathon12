#include <esp_now.h>
#include <DHT.h>
#include <WiFi.h>
#include <ArduinoJson.h>
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define DHT11PIN 5          
#define soilPin 4
DHT dht(DHT11PIN, DHT11);

String recv_jsondata;
String send_jsondata;
StaticJsonDocument<256> doc_to_espnow;
StaticJsonDocument<256> doc_from_espnow;  
const int water = 1159;
const int air = 3000;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void setup() {
  
  dht.begin();
  Serial.begin(115200);
  pinMode(soil,INPUT);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(OnDataSent);            
  
 esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  for (int i = 0; i < 6; i++ )
  {
    peerInfo.peer_addr[i] = (uint8_t) broadcastAddress[i];
  }
  peerInfo.channel = 0;
  peerInfo.encrypt = false;


  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
                                          
  int temp = dht.readTemperature();
  int hum = dht.readHumidity();
  int soilPin = analogRead(soil);
  int moist = map(soilPin,air,water,0,100); 
  doc_to_espnow["temp"] = temp;                   
  doc_to_espnow["humi"] = hum;
  doc_to_espnow["moist"] = moist;                       
  serializeJson(doc_to_espnow, send_jsondata);
  esp_now_send(broadcastAddress, (uint8_t *) send_jsondata.c_str(), send_jsondata.length());
                                                  
  Serial.println(send_jsondata); 
  send_jsondata = "";
  delay(4000);
}