#include<DHT.h>
#include<ESP8266WiFi.h>

String apiKey = "529JT3U7FB5C8C5V";
const char* ssid = "protosem";
const char* password = "proto123";
const char* server = "api.thingspeak.com";

#define DHTTYPE DHT11

const int DHT_pin = D1;
DHT dht(DHT_pin, DHTTYPE);

WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Connecting to WiFi...with SSID: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  Serial.println("Connecting...");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connection successful!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  //Read the temperature
  float t = dht.readTemperature();
  delay(10); //necessary for reading correct values
  //Read the humidity
  float h = dht.readHumidity();

  if(client.connect(server, 80)){
    
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(t);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    delay(1000); //necessary for posting to ThingSpeak

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" centi, Humidity ");
    Serial.print(h);
    Serial.println("%");
    Serial.println("Data sent to ThingSpeak");
  }
  client.stop();

  Serial.println("Waiting for 15 sec...");
  // ThingSpeak needs min. 15 sec delay between each data post
  delay(15000);
}
