/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to PlugPlay MQTT server then:
  - publishes a message containing 3 values (i.e. 0, 1, 2) to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives.

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
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
float data; 

EthernetClient ethClient;
PlugPlayMQTT client(ethClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived --> ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Print device name  
  deviceName = client.getName((char*)payload);
  Serial.print("Device name: ");
  Serial.println(deviceName);

  // Print data values
  data = client.getData((char*)payload,"data0");
  Serial.print("data0: ");
  Serial.println(data);

  data = client.getData((char*)payload,"data1");
  Serial.print("data1: ");
  Serial.println(data);

  data = client.getData((char*)payload,"data2");
  Serial.print("data2: ");
  Serial.println(data);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ArduinoClient-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connectPlugPlay(clientId.c_str())) {
      // Once connected, publish an message...
      Serial.println("connected");
      msg = client.createMsg("arduino", 0, 1, 2);
      client.pubPlugPlay("outTopic", msg);
      
      // ... and resubscribe
      client.subPlugPlay("inTopic");
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
  Serial.begin(57600);

  client.setAuth(USER_KEY, BOARD_ID);
  client.setServer(PLUGPLAY, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
