/*
 Example of using a Stream object to store the message payload

 Uses SRAM library: https://github.com/ennui2342/arduino-sram
 but could use any Stream based class such as SD

  - connects to an MQTT server
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic"
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PlugPlayMQTT.h>
#include <SRAM.h>

#define USER_KEY  "........"
#define BOARD_ID  "........"
#define PLUGPLAY  "mqtt.plugplay.co"

// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(172, 16, 0, 100);

// Init variables
SRAM sram(4, SRAM_1024);
char* msg;

// Callback function header
void callback(char* topic, byte* payload, unsigned int length);

EthernetClient ethClient;
PlugPlayMQTT client(PLUGPLAY, 1883, callback, ethClient, sram);

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

  sram.begin();
  sram.seek(1);

  Serial.begin(9600);
}

void loop()
{
  client.loop();
}

// Callback function
void callback(char* topic, byte* payload, unsigned int length) {
  sram.seek(1);

  // do something with the message
  for(uint8_t i=0; i<length; i++) {
    Serial.write(sram.read());
  }
  Serial.println();

  // Reset position for the next message to be stored
  sram.seek(1);
}