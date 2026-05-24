#include <Arduino.h>
#include "net_tools.h"
#include "credentials.h"
#include "mqtt.h"

void setup() {
    Serial.begin(BAUD);
    net_tools.setup(WIFI_SSID, WIFI_PASS);
    mqtt.setup(MQTT_BROKER, MQTT_PORT);

}

void loop() {
  if (net_tools.is_connected){
    mqtt.maintain();
  }
  else {
    net_tools.reconnect();
    
  }
}