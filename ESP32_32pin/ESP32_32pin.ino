#include <esp_now.h>
#include <DHT.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

#define DHT11PIN 2          
#define soilPin 34
#define highPin 15
#define highPinCap 5
#define lowPinCap 18
DHT dht(DHT11PIN, DHT11);



const int water = 1060;
const int air = 2900;


typedef struct struct_message {
  int temperature;
  int humidity;
  int moisture;
  int board =1;
  int state =0;
} struct_message;
struct_message mydata;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void setup() {
  
  dht.begin();
  Serial.begin(115200);
  pinMode(highPin,OUTPUT);
  pinMode(highPinCap,OUTPUT);
  pinMode(lowPinCap,OUTPUT);
  pinMode(DHT11PIN, INPUT);
  pinMode(soilPin,INPUT);
  
  digitalWrite(highPinCap,HIGH);
  digitalWrite(lowPinCap,LOW);
  digitalWrite(highPin,HIGH);
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
  int humi = dht.readHumidity();
  int soil = analogRead(soilPin);
  int moist = map(soil,air,water,0,100); 
  Serial.print("Temperature : ");
  Serial.println(temp);
  Serial.print("Humidity : ");
  Serial.println(humi);
  Serial.print("Moisture : "); 
  Serial.println(moist);
  mydata.temperature = temp;
  mydata.humidity = humi;
  mydata.moisture = moist;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &mydata, sizeof(mydata));
   if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(4000);
}