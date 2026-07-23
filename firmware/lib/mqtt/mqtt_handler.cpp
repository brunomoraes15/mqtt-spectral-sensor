#include "mqtt.h"
#include <stdlib.h>
#include <string.h>

void MQTT::set_callback(MessageHandler handler){
    _stored_handler = handler;
    _mqtt_client.setCallback(_callback_wrapper);
}

void MQTT::_callback_wrapper(char * topic, byte * payload, unsigned int length){
    if (mqtt._stored_handler == nullptr) {
        return;
    }

    char *message_buffer = static_cast<char *>(malloc(length + 1));
    if (message_buffer == nullptr) {
        return;
    }

    memcpy(message_buffer, payload, length);
    message_buffer[length] = '\0';

    mqtt._stored_handler(topic, message_buffer);
    free(message_buffer);
}
