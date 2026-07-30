#pragma once
#include <Arduino.h>

#define CHECK_INTERVAL 2000

enum class RunState {
    IDLE,
    RUNNING
};

typedef void (*CMDHandler)(const String& args);

typedef struct cmd_entry {
    const char* cmd;
    CMDHandler handler;
} cmd_entry;



class Scheduler {
    public:
        Scheduler();
        void start();
        void check();

        static void cmd_node_start(const String& args);
        static void cmd_node_stop(const String& args);
        static void cmd_node_status(const String& args);
        static void cmd_esp_fheap(const String& args);
        static void cmd_esp_reset_reason(const String& args);
        static void cmd_esp_uptime(const String& args);

        static const char* reset_reason_s(esp_reset_reason_t reason);

        void cmd_exec(const String& command, const String& args);

        static void message_handler(char *, char *);
    private:
        RunState _state = RunState::IDLE;
        unsigned long _sample_count = 0;
        unsigned long _check_timer;
            
};



extern Scheduler scheduler;