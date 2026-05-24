#pragma once

#include <WiFiClient.h>
#include <PubSubClient.h>

#define RETRY_INTERVAL 5000

class MQTT {
    public:
        MQTT();
        void setup(const char * mqtt_broker, int mqtt_port);
        void maintain();

    private:
        WiFiClient _wifi_client;
        PubSubClient _mqtt_client;
        void _log_current_state();
        const char* _get_state(int state);

        unsigned long _retry_timer;
        bool _is_first;

};

extern MQTT mqtt;