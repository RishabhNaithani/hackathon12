
#include <WiFi.h>
#include <esp_now.h>

// #include<ArduinoJson.h>

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 

#define relay1 25
#define relay2 26
#define relay3 27

#define RXD2 16
#define TXD2 17

typedef struct datafromNetwork {
  int temperature;
  int humidity;
  int moisture;
  int board = 0;
  int state = 0;
} datafromNetwork;
datafromNetwork recievedData;



typedef struct datatoNetwork {
  int temp;
  int humi;
  int moist;
} datatoNetwork;
datatoNetwork dataforNetwork;



int checkRelay(int b,int s){
  switch(b){
    case 1:
      if(s==1){
        digitalWrite(relay1, LOW);
        Serial.println("MOTOR 1 ON");
      }
      else{
        digitalWrite(relay1, HIGH);
        Serial.println("MOTOR 1 OFF");
      }
    break;
    case 2:
    if(s==1){
        digitalWrite(relay2, LOW);
        Serial.println("MOTOR 2 ON");
      }
      else{
        digitalWrite(relay2, HIGH);
        Serial.println("MOTOR 2 OFF");
      }
    break;
    case 3:
    if(s==1){
        digitalWrite(relay2, LOW);
        Serial.println("MOTOR 3 ON");
      }
      else{
        digitalWrite(relay2, HIGH);
        Serial.println("MOTOR 3 OFF");
      }
    break;
  }
  return 0;
}


void sendUart(const datafromNetwork &recievedData) {
  uint8_t *dataBytes = (uint8_t *)&recievedData;
  size_t dataSize = sizeof(recievedData);
  Serial2.write(dataBytes, dataSize);
  Serial.flush();  
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&recievedData, incomingData, sizeof(recievedData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Temperature : ");
  Serial.println(recievedData.temperature);
  Serial.print("Moisture : ");
  Serial.println(recievedData.moisture);
  Serial.print("Humidity : ");
  Serial.println(recievedData.humidity);
  Serial.print("Board : ");
  Serial.println(recievedData.board);
  Serial.print("state : ");
  Serial.println(recievedData.state);
  sendUart(recievedData);
  checkRelay(recievedData.board,recievedData.state);
  delay(2000);
}





void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}





void setup() {

  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  myTransfer.begin(Serial2);

  pinMode(relay1,OUTPUT);
  pinMode(relay2,OUTPUT);
  pinMode(relay3,OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);


  esp_now_peer_info_t peerInfo;
  memset(&peerInfo, 0, sizeof(peerInfo));
  for (int i = 0; i < 6; i++)
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







void loop()
{
  
  // esp_now_send(broadcastAddress,(uint8_t *) &dataforNetwork,sizeof(datatoNetwork));

}