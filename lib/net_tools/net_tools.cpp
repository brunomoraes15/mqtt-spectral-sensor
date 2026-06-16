#include "net_tools.h"

NetTools::NetTools(){}

NetTools net_tools;

void NetTools::enable_logging() {
    _echo_allow = true;
}

void NetTools::setup(const char * WIFI_SSID, const char * WIFI_PASS){
    
    WiFi.setAutoReconnect(false);
    WiFi.onEvent(_event_handler);
    WiFi.persistent(false);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    if (_echo_allow && WiFi.status() == WL_CONNECTED){
        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("[WiFi] Connected");
            Serial.print("[WiFi] IP: ");
            Serial.println(WiFi.localIP());
        }
    }
}

void NetTools::reconnect(){
    	if ((millis() - _reconnect_timer > RECONNECT_INTERVAL) && _reconnect_allow) {
		if (_echo_allow) Serial.println("\n\tAttempting to reconnecting\n");
		WiFi.reconnect();
		_reconnect_allow = false;
		_reconnect_timer = millis();
	}     
}

void NetTools::_event_handler(WiFiEvent_t event, WiFiEventInfo_t info) {
	
    switch(event)
    {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        net_tools.is_connected = true;
		if (net_tools._echo_allow){ 
            Serial.println("\tWiFi connected\n");
            Serial.println(WiFi.localIP());}
        break;
	
	case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        net_tools.is_connected = false;
        net_tools._reconnect_allow = true;
        if (net_tools._echo_allow) Serial.println("[WiFi] Disconnected");
        break;

    default:
        break;
	}
}

String NetTools::get_device_mac() {
    String mac_address = WiFi.macAddress(); 
    mac_address.replace(":", "");           
    return mac_address;
}