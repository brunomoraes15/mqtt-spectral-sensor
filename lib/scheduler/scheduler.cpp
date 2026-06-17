#include "scheduler.h"
#include "mqtt.h"

Scheduler::Scheduler() : _check_timer(0) {}
Scheduler scheduler;


void Scheduler::message_handler(char * topic, char * message_buffer){


    Serial.print("\n received: ");
    Serial.print(topic);
    Serial.println(" / ");
    Serial.print(message_buffer);

}

void Scheduler::start(){
    static const char * subs_list[] = {
  "a", "b", "c"
};
    mqtt.set_subscription(subs_list, 3);
    mqtt.set_callback(message_handler);
}

void Scheduler::check(){
    if (millis() - _check_timer > CHECK_INTERVAL){

        _check_timer = millis();
    }
}
