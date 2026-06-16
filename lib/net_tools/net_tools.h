#pragma once

#include <WiFi.h>

#define RECONNECT_INTERVAL 10000


class NetTools {

    public:
        NetTools();
		void enable_logging();
		void setup(const char *, const char *);
		void reconnect();
		String get_device_mac();
        bool is_connected = false;
		

    private:
		bool _reconnect_allow = true;
		bool _first_disconnect = true;
		bool _echo_allow = true;
        unsigned long _reconnect_timer;
		static void _event_handler(WiFiEvent_t, WiFiEventInfo_t);
		
};

extern NetTools net_tools;

