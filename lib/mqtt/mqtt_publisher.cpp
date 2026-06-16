#include "mqtt.h"

void MQTT::publish(const char * topic, const char * payload) {
	_publish(topic, payload);
}

void MQTT::publish(const char * topic, float number) {
	char payload[32];
	dtostrf(number, 0, 2, payload);
	_publish(topic, payload);
}

void MQTT::publish(const char * topic, int number) {
	char payload[32];
	itoa(number, payload, 10);
	_publish(topic, payload);
}


void MQTT::_publish(const char * topic, const char * payload) {
    if (_mqtt_client.connected()) {
        _mqtt_client.publish(topic, payload);
    }
}