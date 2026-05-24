#include "mqtt.h"
#include "net_tools.h"


MQTT::MQTT() {}

MQTT mqtt;

void MQTT::setup(const char * mqtt_broker, int mqtt_port) {
    _mqtt_client.setClient(_wifi_client);
    _mqtt_client.setServer(mqtt_broker, mqtt_port);

}

void MQTT::maintain() {
    if (!_mqtt_client.connected()){
        bool reconnect_allow = _is_first || millis() - _retry_timer > RETRY_INTERVAL;
        if (reconnect_allow){
            _is_first = false;
            Serial.println("Attempting to connect to the broker");
            if (_mqtt_client.connect(_get_device_mac())) {
                Serial.println("MQTT connection stablished");
                _is_first = true;
            }
            else{
                Serial.println("\nFailed to connect to the broker ");
                _log_current_state();
            }

            _retry_timer = millis();
    }
}
    else {
        _mqtt_client.loop();
    }
}

const char* MQTT::_get_state(int state)
{
    switch (state)
    {
        case -4: return "MQTT_CONNECTION_TIMEOUT";
        case -3: return "MQTT_CONNECTION_LOST";
        case -2: return "MQTT_CONNECT_FAILED";
        case -1: return "MQTT_DISCONNECTED";
        case  0: return "MQTT_CONNECTED";
        case  1: return "MQTT_CONNECT_BAD_PROTOCOL";
        case  2: return "MQTT_CONNECT_BAD_CLIENT_ID";
        case  3: return "MQTT_CONNECT_UNAVAILABLE";
        case  4: return "MQTT_CONNECT_BAD_CREDENTIALS";
        case  5: return "MQTT_CONNECT_UNAUTHORIZED";
        default: return "UNKNOWN MQTT STATE";
    }
}

void MQTT::_log_current_state()
{   
    Serial.println(_get_state(_mqtt_client.state()));
}