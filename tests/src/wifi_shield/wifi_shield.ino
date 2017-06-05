/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes a message containing 3 values (i.e. 0, 1, 2) to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

#include <SPI.h>
#include <WiFi.h>
#include <PlugPlayMQTT.h>

char ssid[] = "........";     //  your network SSID (name) 
char password[] = "........";    // your network password
 
#define USER_KEY  "........"
#define BOARD_ID  "........"
#define PLUGPLAY  "mqtt.plugplay.co"

IPAddress mqttServer(192, 168, 2, 38);

WiFiClient wifiClient;
PlugPlayMQTT client(wifiClient);
const char* msg;
long lastMsg = 0;

void setupWifi(void);
void callback(char* topic, byte* payload, unsigned int length);
void reconnect(void);


void setup()
{
    Serial.begin(9600);
    setupWifi();

    Serial.println("Begin connect MQTT");
    client.setAuth(USER_KEY, BOARD_ID);
    client.setServer(PLUGPLAY, 1883);
    client.setCallback(callback);

    // Allow the hardware to sort itself out
    delay(1500);
}

void loop()
{
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    long now = millis();
    if (now - lastMsg > 2000) {
        lastMsg = now;
        //++value;
        //snprintf (msg, 75, "hello world #%ld", value);
        msg = client.createMsg("arduino", 0, 1, 2);
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.pubPlugPlay("mqtt", msg);
    }
}
  

// connect wifiShield to network
void setupWifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i=0;i<length;i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
    Serial.print(client.getName((char*)payload));
    Serial.print(client.getData((char*)payload,"data0"));
    Serial.println();
}

// Reconnect mqttServer when the connection losts
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connectPlugPlay(clientId.c_str())) {
      Serial.println("connected");
      // resubscribe a topic
      client.subPlugPlay("mqtt");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}