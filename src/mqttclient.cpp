#include "headers/blynk.h"
#include "headers/settings.h"
#include "headers/message.h"
#include "headers/utilities.h"

#ifdef TARGET_ESP8266
    #include <ESP8266WiFi.h>
    #include <PubSubClient.h>
    PubSubClient client;
#endif
#ifdef TARGET_ESP32
    #include <AsyncMqttClient.h>
    AsyncMqttClient client;
    #include <WiFi.h>
#endif  

WiFiClient wifiClient;

void mqttSetup()
{
    _sensorSettings = getSettings();
#ifdef TARGET_ESP8266
    client.setClient(wifiClient);
#endif
    client.setServer(_sensorSettings.mqttServer, 1883);
}
 
void mqttLoop(int reading)
{
    if(!client.connected())
    {
#ifdef TARGET_ESP8266
        client.connect(String(getChipName()).c_str(), _sensorSettings.mqttUser, _sensorSettings.mqttPassword);
#endif
#ifdef TARGET_ESP32
        client.setCredentials( _sensorSettings.mqttUser, _sensorSettings.mqttPassword);
        client.connect();
#endif
        delay(500);
    }
    println("Publishing MQTT");
    
    String topic = String("ESP") + _sensorSettings.chipName + String("/temperature/");
    #ifdef TARGET_ESP8266
    client.publish(topic.c_str(), String(reading).c_str(), true);
    client.loop();
    #endif
    #ifdef TARGET_ESP32
    client.publish(topic.c_str(), 0, false, String(reading).c_str(), true);
    #endif
}