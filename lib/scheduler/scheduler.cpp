#include "scheduler.h"
#include "mqtt.h"
#include "sensor.h"
#include "package_builder.h"

Scheduler::Scheduler() : _check_timer(0) {}
Scheduler scheduler;


void Scheduler::message_handler(char * topic, char * message_buffer){
    String command = String(message_buffer);
    command.toLowerCase();  
    
    if (command == "start") {
        Serial.println("started collection");
        scheduler._collection_active = true;
        scheduler._collection_paused = false;
        scheduler._sample_count = 0;
    } 
    else if (command == "pause") {
        Serial.println("paused collection");
        scheduler._collection_paused = true;
    } 
    else if (command == "resume") {
        Serial.println("resumed collection");
        scheduler._collection_paused = false;
    } 
    else if (command == "stop") {
        Serial.println("stopped collection");
        scheduler._collection_active = false;
    } 
    else if (command == "status") {
        String status = scheduler._collection_active ? "ACTIVE" : "INACTIVE";
        status += scheduler._collection_paused ? " (PAUSED)" : "";
        mqtt.publish("device/status", status.c_str());
    }
    else {
        Serial.print("unknown command: ");
        Serial.println(message_buffer);
    }
}

void Scheduler::start(){
static const char *subs_list[] = {
    "sensor/config",
    "sensor/command",
    "sensor/data",
};

    mqtt.set_subscription(subs_list, 3);
    mqtt.set_callback(message_handler);
}

void Scheduler::check(){
    if (millis() - _check_timer > CHECK_INTERVAL){
        _check_timer = millis();

        if (_collection_active && !_collection_paused) {
           // _sample_count++;
        }
    
    }
}
