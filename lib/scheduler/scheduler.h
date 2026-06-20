#pragma once
#include <Arduino.h>
#define CHECK_INTERVAL 2000

class Scheduler {
    public:
        Scheduler();
        void start();
        void check();

        static void message_handler(char *, char *);
    private:
        bool _collection_active = false;
        bool _collection_paused = false;
        unsigned long _sample_count = 0;
        unsigned long _check_timer;
            
};
extern Scheduler scheduler;