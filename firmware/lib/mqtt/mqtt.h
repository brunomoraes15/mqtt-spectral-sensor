#pragma once

#include <WiFiClient.h>
#include <PubSubClient.h>
//#include "net_tools.h"

#define RETRY_INTERVAL 5000

typedef void (*MessageHandler)(char *, char *);

class MQTT {
    public:
        MQTT();
        void setup(const char * mqtt_broker, int mqtt_port);
        void maintain();
        void publish(const char * topic, const char * payload);
        void publish(const char * topic, int number);
        void publish(const char * topic, float number);

        void set_subscription(const char **, int);
        void set_callback(MessageHandler);

        bool _is_connected = false;
    private:
        WiFiClient _wifi_client;
        PubSubClient _mqtt_client;

        void _subs_to_all();
        void _log_current_state();
        void _publish(const char * topic, const char * payload);
        
        const char* _get_state(int state);
        const char ** _subs_list = nullptr;

        bool _is_first;
        bool _echo_allow = true;
        bool _initial_sub = true;
        unsigned long _retry_timer;
        int _sub_list_length = 0;

        MessageHandler _stored_handler = nullptr;
        static void _callback_wrapper(char * topic, byte * payload, unsigned int length);

        
};

extern MQTT mqtt;
