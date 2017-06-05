/*
 Basic MQTT example with Authentication

  - connects to PlugPlay MQTT server, setting authentication 
  with userKey and boardId
  - publishes a message containing 3 values (i.e. 0, 1, 2) to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PlugPlayMQTT.h>

#define USER_KEY  "........"
#define BOARD_ID  "........"
#define PLUGPLAY  "mqtt.plugplay.co"

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(172, 16, 0, 100);

// Init variables
char* msg; 

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PlugPlayMQTT client(PLUGPLAY, 1883, callback, ethClient);

void setup()
{
  Ethernet.begin(mac, ip);

  // Set PlugPlay Authentication
  client.setAuth(USER_KEY, BOARD_ID);

  // Create a message
  msg = client.createMsg("arduino", 0, 1, 2);
  
  // Create a random client ID
  String clientId = "ArduinoClient-";
  clientId += String(random(0xffff), HEX);

  if (client.connectPlugPlay(clientId.c_str())) {
    client.pubPlugPlay("outTopic", msg);
    client.subPlugPlay("inTopic");
  }
}

void loop()
{
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}