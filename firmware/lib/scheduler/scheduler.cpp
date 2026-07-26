#include "scheduler.h"
#include "mqtt.h"
#include "sensor.h"
#include "package_builder.h"

Scheduler::Scheduler() : _check_timer(0) {}
Scheduler scheduler;


void Scheduler::message_handler(char * topic, char * message_buffer)
{
    char* args = strchr(message_buffer, ' ');
    if (args != nullptr)
    {
        *args = '\0';
        args++;
    }
    scheduler.cmd_exec(message_buffer, args);
}

void Scheduler::cmd_exec(const String& command, const String& args)
{
    for (size_t i = 0; i < NUM_CMD; i++)
    {
        if (command.equalsIgnoreCase(cmds[i].cmd))
        {
            cmds[i].handler(args);
            return;
        }
    }

    Serial.println("Unknown command");
}

void Scheduler::cmd_start(const String&)
{
    scheduler._state = RunState::RUNNING;
    scheduler._sample_count = 0;
}

void Scheduler::cmd_stop(const String&)
{
    scheduler._state = RunState::IDLE;
}

void Scheduler::cmd_stop(const String&)
{
    String status = scheduler._state == RunState::RUNNING ? "ACTIVE" : "INACTIVE";
    mqtt.publish("device/status", status.c_str());
}
void Scheduler::start(){

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
