#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(D4,D8); // CE, CSN
const byte address[6] = "00001";
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
 
//-------- Customise these values -----------
const char* ssid = "MLRITM@DGL";
const char* password = "digi@123";
#define ORG "pxft16"
#define DEVICE_TYPE "nodemcu32"
#define DEVICE_ID "kammasairam"
#define TOKEN "0123456789"
char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char pubtopic[] = "iot-2/evt/dht11/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);
void setup() {
  Serial.begin(115200);
   Serial.println();
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
   Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}

void loop() {  
  float t,h;
  if (radio.available()) {
    radio.read(&t, sizeof(t));
    Serial.print("temperature: ");
    Serial.println(t);
    delay(2000);
     radio.read(&h, sizeof(h));
     Serial.print("humidity: ");
    Serial.println(h);
    delay(2000);
  }
  PublishData(t,h);
delay(1000);
}

void PublishData(float t, float h){
 if (!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += t;
  payload+="," "\"humidity\":";
  payload += h;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(pubtopic, (char*) payload.c_str())) 
 {
 Serial.println("Publish ok");
 } 
 else 
 {
 Serial.println("Publish failed");
 }
}
