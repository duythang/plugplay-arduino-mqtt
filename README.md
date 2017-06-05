# Arduino Client for MQTT PlugPlay

This library provides a client for doing simple publish/subscribe messaging with MQTT PlugPlay.

This library is derived almost entirely from the great work done here: https://github.com/knolleary/pubsubclient

## Examples

The library comes with a number of example sketches. See File > Examples > PlugPlayMQTT
within the Arduino application.

## Limitations

 - It can only publish QoS 0 messages. It can subscribe at QoS 0 or QoS 1.
 - The maximum message size, including header, is **128 bytes** by default. This
   is configurable via `MQTT_MAX_PACKET_SIZE` in `PubSubClient.h`.
 - The keepalive interval is set to 15 seconds by default. This is configurable
   via `MQTT_KEEPALIVE` in `PubSubClient.h`.
 - The client uses MQTT 3.1.1 by default. It can be changed to use MQTT 3.1 by
   changing value of `MQTT_VERSION` in `PubSubClient.h`.

## Compatible Hardware

The library uses the Arduino Ethernet Client api for interacting with the
underlying network hardware. This means it Just Works with a growing number of
boards and shields, including:

 - Arduino Ethernet
 - Arduino Ethernet Shield
 - Arduino YUN – use the included `YunClient` in place of `EthernetClient`, and
   be sure to do a `Bridge.begin()` first
 - Arduino WiFi Shield - if you want to send packets > 90 bytes with this shield,
   enable the `MQTT_MAX_TRANSFER_SIZE` define in `PubSubClient.h`.
 - Sparkfun WiFly Shield – [library](https://github.com/dpslwk/WiFly)
 - TI CC3000 WiFi - [library](https://github.com/sparkfun/SFE_CC3000_Library)
 - Intel Galileo/Edison
 - ESP8266

The library cannot currently be used with hardware based on the ENC28J60 chip –
such as the Nanode or the Nuelectronics Ethernet Shield. For those, there is an
[alternative library](https://github.com/njh/NanodeMQTT) available.

## Install

Clone (or download and unzip) the repository to `~/Documents/Arduino/libraries`
where `~/Documents/Arduino` is your sketchbook directory.

    > cd ~/Documents/Arduino
    > mkdir libraries
    > cd libraries
    > git clone https://github.com/plugplayco/plugplay-arduino-mqtt PlugPlayMQTT

## Usage

### setAuth( const char* userKey, const char* boardId)

Set authentication to connect to PlugPlay

Arguments: 
- *userKey* - user Key of your user account.
- *boardId* - board Id of board you want to publish/subcribe to.

### boolean connectPlugPlay(const char* id)

Connect to PlugPlay with a MQTT client Id

Arguments: 
- *id* - client Id (should be a random Id)

```c++
// Create a random client ID
String clientId = "ArduinoClient-";
clientId += String(random(0xffff), HEX);

client.connectPlugPlay(clientId.c_str());
```
### char* createMsg(String dvName, float data0, float data1, float data2)

Create a message to publish

Arguments: 
- *dvName* - device name
- *data0* - first data
- *data1* - second data
- *data2* - third data

Other functions:
- **char* createMsg(String dvName, float data0)** - data1 and data2 are set to 0 
- **char* createMsg(String dvName, float data0, float data1)** - data2 is set to 0

### boolean subPlugPlay(const char* topic)

Subcribe a topic 

### boolean pubPlugPlay(const char* topic, const char* payload)

Publish a message to a topic. The message is created by *createMsg(String dvName, float data0, float data1, float data2)*

### String getName(const char* payload)

Get device name from a received message

### float getData(const char* payload, const char* type)

Get data from a received message

```c++
// Get the first data
client.getData((char*)payload,"data0");
// Get the second data
client.getData((char*)payload,"data1");
// Get the third data
client.getData((char*)payload,"data2");
```

### Other functions

You can refer here: http://pubsubclient.knolleary.net 

## License

This code is released under the MIT License.
