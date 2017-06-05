/*
 Reconnecting MQTT example - non-blocking

 This sketch demonstrates how to keep the client connected
 using a non-blocking reconnect function. If the client loses
 its connection, it attempts to reconnect every 5 seconds
 without blocking the main loop.

*/

#include <SPI.h>
#include <Ethernet.h>
#include <PlugPlayMQTT.h>

#define USER_KEY  "........"
#define BOARD_ID  "........"
#define PLUGPLAY  "mqtt.plugplay.co"

// Update these with values suitable for your hardware/network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(172, 16, 0, 100);

// Init variables
char* msg;

EthernetClient ethClient;
PlugPlayMQTT client(ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

long lastReconnectAttempt = 0;

boolean reconnect() {
  // Create a message
  msg = client.createMsg("arduino", 0, 1, 2);
  
  // Create a random client ID
  String clientId = "ArduinoClient-";
  clientId += String(random(0xffff), HEX);
  
  if (client.connectPlugPlay(clientId.c_str())) {
    // Once connected, publish an announcement...
    Serial.println("connected");
    client.pubPlugPlay("outTopic", msg);
    
    // ... and resubscribe
    client.subPlugPlay("inTopic");
  }
  return client.connected();
}

void setup()
{
  client.setAuth(USER_KEY, BOARD_ID);
  client.setServer(PLUGPLAY, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  delay(1500);
  lastReconnectAttempt = 0;
}


void loop()
{
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Client connected

    client.loop();
  }

}
