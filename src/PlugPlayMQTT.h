#ifndef PlugPlayMQTT_h
#define PlugPlayMQTT_h

#include <PubSubClient.h>
#include <ArduinoJson.h>

// MQTT_MAX_PAYLOAD_SIZE : Maximum payload size inside a packet
#ifndef MQTT_MAX_PAYLOAD_SIZE
#define MQTT_MAX_PAYLOAD_SIZE 100
#endif

class PlugPlayMQTT: public PubSubClient {

    private:
        const char* _userKey; 
        const char* _boardId;
        String _authSubTopic, _authPubTopic;
        char payload[MQTT_MAX_PAYLOAD_SIZE];
    public:
        //PlugPlayMQTT(Client& client):PubSubClient(client){};
        using PubSubClient::PubSubClient;
        PlugPlayMQTT& setAuth( const char* userKey, const char* boardId);
        boolean connectPlugPlay(const char* id);
        boolean subPlugPlay(const char* topic);
        boolean pubPlugPlay(const char* topic, const char* payload);
        
        char* createMsg(String dvName, float data0);
        char* createMsg(String dvName, float data0, float data1);
        char* createMsg(String dvName, float data0, float data1, float data2);
        
        String   getName(const char* payload);
        float    getData(const char* payload, const char* type);
};

#endif