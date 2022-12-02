#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h> 
#include <PubSubClient.h>   
#include <ArduinoOTA.h>

const byte relais_A = D4;       


// Kartendaten

const char* kartenID = "24V_Weihnachten_Lichter";

WiFiClient espClient;
PubSubClient client(espClient);

// Connect to the WiFi
const char* ssid = "GuggenbergerLinux";
const char* password = "Isabelle2014samira";
const char* mqtt_server = "192.168.150.1";


void callback(char* topic, byte* payload, unsigned int length) {


    if (strcmp(topic,"Relaiskarte/Weihnachten/24V_1")==0) {

        // Kanal A
        if ((char)payload[0] == 'o' && (char)payload[1] == 'n') {  
                 Serial.println("relais_A -> AN");
                 digitalWrite(relais_A, !HIGH);
                 //client.publish("Relaiskarte/Weihnachten/24V_1","on");
                delay(100);
              }

        if ((char)payload[0] == 'o' && (char)payload[1] == 'f' && (char)payload[2] == 'f') {  
                 Serial.println("relais_A -> AUS");
                 digitalWrite(relais_A, !LOW);
                 //client.publish("Relaiskarte/Weihnachten/24V_1","off");
                delay(100);
              }
      } 
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("connect mqtt IP: ");
    Serial.print(mqtt_server);
    Serial.println("");
    // Create a random client ID
    String clientId = "WeihnachtenRelaiskarte-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("Verbunden ....");
      // ... and resubscribe
      client.subscribe("Relaiskarte/Weihnachten/24V_1");
          
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {

//Pins deklarieren
  pinMode(relais_A, OUTPUT);

// Alle Relais auf LOW setzen

  digitalWrite(relais_A, !LOW);

// MQTT Broker
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

// Serielle Kommunikation starten
  Serial.begin(115200);

  // WiFi 
  IPAddress ip(192, 168, 33, 15);
	IPAddress dns(192, 168, 1, 1);  
	IPAddress subnet(255, 255, 0, 0);
	IPAddress gateway(192, 168, 1, 1);
	WiFi.config(ip, dns, gateway, subnet);
  WiFi.begin(ssid, password);

  // Wifi AP deaktivieren
  WiFi.mode(WIFI_STA);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi connected...");


// Start mit Relais klack bestaetigen

digitalWrite(relais_A, !LOW);
delay(300);
digitalWrite(relais_A, !HIGH);
delay(300);
digitalWrite(relais_A, !LOW);
delay(300);
digitalWrite(relais_A, !HIGH);
delay(300);
digitalWrite(relais_A, !LOW);
}


void loop() {

    // OTA Handle starten
  ArduinoOTA.handle(); 

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

delay(300);

  
}

