#pragma once
#include <Arduino.h>

#define CHECK_INTERVAL 2000

enum class RunState {
    IDLE,
    RUNNING
};

class Scheduler {
    public:
        Scheduler();
        void start();
        void check();

        static void message_handler(char *, char *);
    private:
        RunState _state = RunState::IDLE;
        unsigned long _sample_count = 0;
        unsigned long _check_timer;
            
};



extern Scheduler scheduler;