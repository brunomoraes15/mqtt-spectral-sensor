#include <Arduino.h>
#include "net_tools.h"
#include "credentials.h"
#include "mqtt.h

// put function declarations here:
int myFunction(int, int);

void setup() {
    Serial.begin(BAUD);
    Serial.println("Started");
    NetTools.setup(WIFI_SSID, WIFI_PASS);
    mqtt.setup(MQTT_BROKER, MQTT_PORT);

}

void loop() {
  if (NetTools.is_connected){
    mqtt.maintain();
  }
  else {
    NetTools.reconnect();
    
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}