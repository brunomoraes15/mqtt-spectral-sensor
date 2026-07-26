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

        static void cmd_start(const String& args);
        static void cmd_stop(const String& args);
        static void cmd_status(const String& args);

        void cmd_exec(const String& command, const String& args);

        static void message_handler(char *, char *);
        inline static const char *subs_list[] = {
            "sensor/config",
            "sensor/command",
            "sensor/data",
        };
    private:
        RunState _state = RunState::IDLE;
        unsigned long _sample_count = 0;
        unsigned long _check_timer;
            
};

cmd_entry cmds[] = {
    { "start", Scheduler::cmd_start },
    { "stop", Scheduler::cmd_stop },
    { "status", Scheduler::cmd_status }
};
const int NUM_CMD = sizeof(cmds) / sizeof(cmds[0]);

extern Scheduler scheduler;