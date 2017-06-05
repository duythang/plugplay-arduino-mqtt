#include "PlugPlayMQTT.h"
#include "Arduino.h"

PlugPlayMQTT& PlugPlayMQTT::setAuth( const char* userKey, const char* boardId) {
    this->_userKey = userKey;
    this->_boardId = boardId;
    return *this;
}

boolean PlugPlayMQTT::connectPlugPlay(const char* id) {
    //Note: In connect(), when passw is available, user has to be available (!= NULL)
    return connect(id,_userKey,_boardId);
}

boolean PlugPlayMQTT::subPlugPlay(const char* topic) {
    _authSubTopic = String(_boardId) + "/" + String(topic);
    return subscribe(_authSubTopic.c_str(), 0);
}

boolean PlugPlayMQTT::pubPlugPlay(const char* topic, const char* payload) {
    _authPubTopic = String(_boardId) + "/" + String(topic);
    return publish(_authPubTopic.c_str(),(const uint8_t*)payload,strlen(payload),false);
}

char* PlugPlayMQTT::createMsg(String dvName, float data0){    
    return createMsg(dvName, data0, 0, 0);
}

char* PlugPlayMQTT::createMsg(String dvName, float data0, float data1){    
    return createMsg(dvName, data0, data1, 0);
}

char* PlugPlayMQTT::createMsg(String dvName, float data0, float data1, float data2){    
    StaticJsonBuffer<MQTT_MAX_PAYLOAD_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject(); 
    root["device"] = dvName;
    root["data0"] = data0;
    root["data1"] = data1;
    root["data2"] = data2;
    root.printTo(payload, MQTT_MAX_PAYLOAD_SIZE);
    return payload;
}

String PlugPlayMQTT::getName(const char* payload){
    StaticJsonBuffer<MQTT_MAX_PAYLOAD_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
        Serial.println("parseMsg() failed");
        return "";
    }
    return root["device"];
} 

float PlugPlayMQTT::getData(const char* payload, const char* type){
    StaticJsonBuffer<MQTT_MAX_PAYLOAD_SIZE> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(payload);
    if (!root.success()) {
        Serial.println("parseMsg() failed");
        return 0;
    }    
    return root[type];
}