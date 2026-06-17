#pragma once

#define CHECK_INTERVAL 2000
#include <Arduino.h>
class Scheduler {
    public:
        Scheduler();
        void start();
        void check();

        static void message_handler(char *, char *);
    private:
        unsigned long _check_timer;
            
};
extern Scheduler scheduler;