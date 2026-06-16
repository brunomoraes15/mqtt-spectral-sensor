#pragma once

#include <WiFiClient.h>
#include <PubSubClient.h>
//#include "net_tools.h"

#define RETRY_INTERVAL 5000

class MQTT {
    public:
        MQTT();
        void setup(const char * mqtt_broker, int mqtt_port);
        void maintain();
        void publish(const char * topic, const char * payload);
        void publish(const char * topic, int number);
        void publish(const char * topic, float number);
        bool _is_connected;
    private:
        WiFiClient _wifi_client;
        PubSubClient _mqtt_client;
        void _log_current_state();
        const char* _get_state(int state);
        unsigned long _retry_timer;
        bool _is_first;
        void _publish(const char * topic, const char * payload);
        
};

extern MQTT mqtt;