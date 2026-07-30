#include "scheduler.h"
#include "mqtt.h"
#include "sensor.h"
#include "package_builder.h"

Scheduler::Scheduler() : _check_timer(0) {}
Scheduler scheduler;

cmd_entry cmds[] = {
    { "start", Scheduler::cmd_node_start },
    { "stop", Scheduler::cmd_node_stop },
    { "status", Scheduler::cmd_node_status },
    { "fheap", Scheduler::cmd_esp_fheap },
    { "reset_reason", Scheduler::cmd_esp_reset_reason },
    { "uptime", Scheduler::cmd_esp_uptime },
    
};

static const char *subs_list[] = {
            "sensor/config",
            "sensor/command",
            "sensor/data",
        };

const int NUM_CMD = sizeof(cmds) / sizeof(cmds[0]);

const char* resetReasonToString(esp_reset_reason_t reason)
{
    switch (reason)
    {
        case ESP_RST_POWERON:   return "Power on";
        case ESP_RST_EXT:       return "External";
        case ESP_RST_SW:        return "Software";
        case ESP_RST_PANIC:     return "Panic";
        case ESP_RST_INT_WDT:   return "Interrupt WDT";
        case ESP_RST_TASK_WDT:  return "Task WDT";
        case ESP_RST_WDT:       return "Other WDT";
        case ESP_RST_DEEPSLEEP: return "Deep Sleep";
        case ESP_RST_BROWNOUT:  return "Brownout";
        case ESP_RST_SDIO:      return "SDIO";
        default:                return "Unknown";
    }
}

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
}

void Scheduler::cmd_node_start(const String&)
{
    scheduler._state = RunState::RUNNING;
    scheduler._sample_count = 0;
}

void Scheduler::cmd_node_stop(const String&)
{
    scheduler._state = RunState::IDLE;
}

void Scheduler::cmd_node_status(const String&)
{
    String status = scheduler._state == RunState::RUNNING ? "ACTIVE" : "INACTIVE";
    mqtt.publish("device/status", status.c_str());
}

void Scheduler::cmd_esp_fheap(const String&)
{
    const String fheap = String(esp_get_free_heap_size());

    mqtt.publish("device/config", fheap.c_str());
    Serial.println("Free heap size: " + fheap);
}

void Scheduler::cmd_esp_reset_reason(const String&)
{
    esp_reset_reason_t reset_reason = esp_reset_reason();
    const char* reset_reason_str = Scheduler::reset_reason_s(reset_reason);
    Serial.println("Reset reason: " + String(reset_reason_str));
}

void Scheduler::cmd_esp_uptime(const String& args){
    uint64_t uptime_sec = esp_timer_get_time() / 1000000ULL;
    uint64_t s = uptime_sec;

    uint32_t days = s / 86400;
    s %= 86400;
    uint32_t hours = s / 3600;
    s %= 3600;
    uint32_t minutes = s / 60;
    uint32_t seconds = s % 60;

    Serial.printf("%ud %uh %um %us\n",days, hours, minutes, seconds);
}

// Escalonador
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
