#include <Arduino.h>
#include "net_tools.h"
#include "credentials.h"
#include "mqtt.h"
#include "scheduler.h"
#include "sensor.h"


void setup() {
    net_tools.enable_logging();
    Serial.begin(BAUD);

    if (!sensor.begin()) {
        Serial.println("Failed to initialize AS7341");
    }

    scheduler.start();
    net_tools.setup(WIFI_SSID, WIFI_PASS);
    mqtt.setup(MQTT_BROKER, MQTT_PORT);
}

void loop() {
  mqtt.maintain();

  if (net_tools.is_connected){
    scheduler.check();
  }
  else {
    net_tools.reconnect();
  }

  //mqtt.publish("test_topic", "test_payload");
  //delay(1000);
}