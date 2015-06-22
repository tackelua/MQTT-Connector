
#define DEBUG_MODE

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <MqttWrapper.h>
#include <PubSubClient.h>

// const char* ssid     = "OpenWrt_NAT_500GP.101";
// const char* pass = "activegateway";

const char* ssid     = "MAKERCLUB-CM";
const char* pass = "welcomegogogo";

// const char* ssid     = "Opendream Play";
// const char* pass = "5k,skrijv',7'sik";

// const char* ssid     = "Opendream";
// const char* pass = "gfkgvkgv'2015!!!!";

#define WIFI_MAX_RETRIES 1500
#define WIFI_CONNECT_DELAY_MS 20

MqttWrapper *mqtt;



void connect_wifi()
{
    WiFi.begin(ssid, pass);

    int retries = 0;
    while ((WiFi.status() != WL_CONNECTED))
    {
        Serial.print(".");
        retries++;
        delay(500);
    }

    Serial.println("WIFI CONNECTED ");
}

void reconnect_wifi_if_link_down() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WIFI DISCONNECTED");
        connect_wifi();
    }
}

void callback(const MQTT::Publish& pub) {
    if (pub.payload_string() == "0") {
        Serial.print(" => ");
        Serial.println(pub.payload_string());
    }
    else if(pub.payload_string() == "1") {
        Serial.print(" => ");
        Serial.println(pub.payload_string());
    }
    else {
        Serial.print(pub.topic());
        Serial.print(" => ");
        Serial.println(pub.payload_string());
    }
}

void hook_before_publish(JsonObject** root) {
  JsonObject& data = (*(*root))["d"];

  data["myName"] = "NAT";
}

void setup() {
    Serial.begin(115200);
    pinMode(0, INPUT_PULLUP);
    delay(10);

    connect_wifi();

    mqtt = new MqttWrapper("128.199.104.122");
    mqtt->connect(callback);
    mqtt->set_prepare_publish_data_hook(hook_before_publish, 5000);
}



void loop() {
    reconnect_wifi_if_link_down();
    mqtt->loop();

    if (digitalRead(0) == LOW) {
        while(digitalRead(0) == LOW) { 
            mqtt->loop();
            yield(); 
        }
        mqtt->sync_pub(String("0"));
    }

}
