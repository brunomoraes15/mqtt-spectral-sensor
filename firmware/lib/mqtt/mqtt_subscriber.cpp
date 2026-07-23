#include "mqtt.h"

void MQTT::set_subscription(const char ** topics, int count){
    _subs_list = topics;
    _sub_list_length = count;
}

void MQTT::_subs_to_all() {
    if (_subs_list){
        for (int i = 0; i < _sub_list_length; i++){
            if(_echo_allow && _initial_sub){
                Serial.print("\t sub to: ");
                Serial.println(_subs_list[i]);
            }
            
            _mqtt_client.subscribe(_subs_list[i]);
        }
        _initial_sub = false;
    }
}
