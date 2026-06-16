#include <Arduino.h>
#include "net_tools.h"
#include "credentials.h"
#include "mqtt.h"
#include "scheduler.h"

void setup() {
    net_tools.enable_logging();
    Serial.begin(BAUD);
    scheduler.start();
    net_tools.setup(WIFI_SSID, WIFI_PASS);
    mqtt.setup(MQTT_BROKER, MQTT_PORT);

}

void loop() {
  if (net_tools.is_connected){
    mqtt.maintain();
    scheduler.check();
  }
  else {
    net_tools.reconnect();
    //mqtt.is_connected = false;
  }
  mqtt.publish("test_topic", "test_payload");
  delay(1000);
}