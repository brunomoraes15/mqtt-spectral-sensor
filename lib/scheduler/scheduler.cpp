#include "scheduler.h"
#include "mqtt.h"

Scheduler::Scheduler() {}
Scheduler scheduler;

void Scheduler::start(){
}

void Scheduler::check(){
    if (millis() - _check_timer > CHECK_INTERVAL){

        _check_timer = millis();
    }
}
