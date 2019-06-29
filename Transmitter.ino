#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "DHT.h"
#define DHTPIN D2 
#define DHTTYPE DHT11 

RF24 radio(D4,D8); // CE, CSN
const byte address[6] = "00001";
DHT dht(DHTPIN, DHTTYPE);


void setup()
{
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.begin(115200);
  dht.begin();
}

void loop()
{

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
 
  if (isnan(h) || isnan(t) )
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
  delay(5);
  radio.stopListening();  
  //const char text[] = "&h";
  radio.write(&t, sizeof(t));
  delay(2000);
  radio.write(&h, sizeof(h));
  delay(2000);
}
