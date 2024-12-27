#include <WiFi.h>
#include <PubSubClient.h>

#define EXTERNAL_LED 23
#define EXTERNAL_LED_BUILTIN 2

String wifiSSID = "Namah Shivaya 2.4G_EXT";
String wifiPassword = "HailValavan";
String mqttBroker = "test.mosquitto.org";
WiFiClient client;
PubSubClient mqtt(client);

void connectWifi();
void connect_mqtt();
void mqttReceivedMessage(char *topic, byte *msg, unsigned int msgLength);

void setup()
{
    pinMode(EXTERNAL_LED, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600);
    connectWifi();
    mqtt.setServer(mqttBroker.c_str(), 1883);
    mqtt.setCallback(mqttReceivedMessage);
}

void loop()
{
    if (!mqtt.connected())
    {
        connect_mqtt();
        Serial.println("MQTT Connected");
        mqtt.publish("device/ESP32", "ESP 32 Online!");
    }
    mqtt.loop();
}

void mqttReceivedMessage(char *topic, byte *msg, unsigned int msgLength)
{
    if (String(topic) == "device/LED")
    {
        Serial.println(topic);
        String perintah = "";
        for (int i = 0; i < msgLength; i++)
        {
            Serial.print(char(msg[i]));
            perintah += String(char(msg[i]));
        }
        Serial.println("");
        Serial.println(perintah);
        if (perintah == "ON")
        {
            digitalWrite(EXTERNAL_LED, 1);
            digitalWrite(LED_BUILTIN, 1);
        }
        else if (perintah == "OFF")
        {
            digitalWrite(EXTERNAL_LED, 0);
            digitalWrite(LED_BUILTIN, 0);
        }
    }
}

void connect_mqtt()
{
    while (!mqtt.connected())
    {
        Serial.println("Connecting MQTT...");
        if (mqtt.connect("device"))
        {
            mqtt.subscribe("device/LED");
            mqtt.subscribe("device/ESP32");
        }
    }
}

void connectWifi()
{
    Serial.println("Connecting To Wifi");
    WiFi.begin(wifiSSID.c_str(), wifiPassword.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("Wifi Connected");
    Serial.println(WiFi.SSID());
    Serial.println(WiFi.RSSI());
    Serial.println(WiFi.macAddress());
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.dnsIP());
}