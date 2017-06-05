/*
 Publishing in the callback

  - connects to an MQTT server
  - subscribes to the topic "inTopic"
  - when a message is received, republishes it to "outTopic"

  This example shows how to publish messages within the
  callback function. The callback function header needs to
  be declared before the PubSubClient constructor and the
  actual callback defined afterwards.
  This ensures the client reference in the callback function
  is valid.

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
String deviceName;
float data0, data1, data2;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PlugPlayMQTT client(PLUGPLAY, 1883, callback, ethClient);

void setup()
{

  Ethernet.begin(mac, ip);
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

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  // Copy values in playload and create a new message to publish
  deviceName = client.getName((char*)payload);
  data0 = client.getData((char*)payload,"data0");
  data1 = client.getData((char*)payload,"data1");
  data2 = client.getData((char*)payload,"data2");
  msg = client.createMsg(deviceName, data0, data1, data2);

  client.pubPlugPlay("outTopic", msg);
}