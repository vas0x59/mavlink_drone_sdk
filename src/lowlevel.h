// #include "helpers.h"

#ifndef LOWLEVEL_H
#define LOWLEVEL_H

#include "mavlink.h"
#include <string>

using namespace std;

namespace mavlink_indoor_sdk
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
    // void start(){};
    // void stop(){};
};
};

#endif