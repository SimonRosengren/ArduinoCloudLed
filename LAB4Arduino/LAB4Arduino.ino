#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
 
const char* ssid = "Nael";
const char* password = "nael1010";
 
const char* mqttServer = "m23.cloudmqtt.com";

const int mqttPort = 17686;
const char* mqttUser = "npbmncla";
const char* mqttPassword = "hnCSQbbV2Q8M";
 
int ledPin = 13; // GPIO13
WiFiServer server(80);

WiFiClient espClient;
PubSubClient client(espClient);


void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i=0;i<length;i++) {
  char receivedChar = (char)payload[i];
  Serial.print(receivedChar);
  if (receivedChar == '0'){
      Serial.print("IS ON");
      digitalWrite(ledPin, HIGH);
    }

  if (receivedChar == '1')
   digitalWrite(ledPin, LOW);
  }
  Serial.println();
}

 void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("ESP8266 Client", mqttUser, mqttPassword)) {
  Serial.println("connected");
  // ... and subscribe to topic
  client.subscribe("LED");
 } else {
  Serial.print("failed, rc=");
  Serial.print(client.state());
  Serial.println(" try again in 5 seconds");
  // Wait 5 seconds before retrying
  delay(5000);
  }
 }
}

void setup()
{

 Serial.begin(115200);
  setup_wifi();
 delay(5000);
 client.setServer(mqttServer, 17686);
 client.setCallback(callback);
 
 pinMode(ledPin, OUTPUT);
}
 
void loop()
{
 if (!client.connected()) {
  reconnect();
 }
 client.loop();
}
