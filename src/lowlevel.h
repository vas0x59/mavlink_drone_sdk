// #include "helpers.h"

#ifndef LOWLEVEL_H
#define LOWLEVEL_H

#include "mavlink.h"
#include <string>
#include "log.h"

using namespace std;

namespace mavlink_drone_sdk
{
class LowLevel
{
public:
    // LowLevel(string url){};
    virtual void start(){};
    virtual void stop(){};

    virtual int read_message(mavlink_message_t &message){return 0;};
    virtual int write_message(mavlink_message_t &message){return 0;};
    int status = 1;
    virtual void handle_quit(int sig){};
    // void start(){};
    // void stop(){};
};
};

#endif