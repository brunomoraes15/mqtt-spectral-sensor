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
        scheduler._state = RunState::RUNNING;
        scheduler._sample_count = 0;
    } 
    else if (command == "stop") {
        Serial.println("stopped collection");
        scheduler._state = RunState::IDLE;
    } 
 
    else if (command == "status") {
        String status = scheduler._state == RunState::RUNNING ? "ACTIVE" : "INACTIVE";
        mqtt.publish("device/status", status.c_str());
    }
    else {
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

        if (_state == RunState::RUNNING) {
           sensor_sample sample;
           if (sensor.read(sample)){
            String payload = Payload::build_payload(sample);
            mqtt.publish("sensor/data", payload.c_str());
            _sample_count++;
           }
           else {
            Serial.println("Sensor read failed");
           }
        }
    
    }
}
