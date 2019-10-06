// by Vasily Yuryev

#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <vector>
#include <sys/time.h>
#include <pthread.h> // This uses POSIX Threads
#include <cstdarg>
#include <string>
#include <cmath>

#include "mavlink.h"
#include "px4_custom_mode.h"
#include "log.h"

using namespace std;
namespace mavlink_drone_sdk
{

uint64_t get_time_usec();
uint64_t get_time_msec();

enum Mode
{
    MODE_MANUAL,
    MODE_ALTITUDE,
    MODE_POSITION,
    MODE_MISSION,
    MODE_HOLD,
    MODE_RETURN,
    MODE_ACRO,
    MODE_OFFBOARD,
    MODE_STABILIZE,
    MODE_RATTITUDE,
    MODE_TAKEOFF,
    MODE_LAND,
    MODE_FOLLOW_ME,
    MODE_UNASSIGNED
};


struct PointXYZ
{
    PointXYZ(){};
    PointXYZ(float x_, float y_, float z_);
    float x;
    float y;
    float z;
    // string to_string
    string ToString();
    // operator
};

PointXYZ operator+(PointXYZ q, PointXYZ r);
PointXYZ operator-(PointXYZ q, PointXYZ r);
PointXYZ operator*(PointXYZ q, PointXYZ r);
PointXYZ operator/(PointXYZ q, PointXYZ r);

struct RotationRPY
{
    RotationRPY(){};
    RotationRPY(float r_, float p_, float y_);
    float roll;
    float pitch;
    float yaw;

    string ToString();
};
struct Battery
{
    Battery(){};
    Battery(mavlink_battery_status_t mav_bat);
    int cells_count = 0;
    std::vector<float> cells_voltage;
    float voltage = 0;
    float remaining = 0;
    string ToString();
};
struct PointXYZyaw
{
    PointXYZyaw(){};
    PointXYZyaw(float x_, float y_, float z_, float yaw_);
    float x;
    float y;
    float z;
    float yaw;

    string ToString();
};
enum Frame
{
    FRAME_VISION,
    FRAME_LOCAL,
    FRAME_BODY
};

struct Telemetry
{
    PointXYZ position;
    PointXYZ velocity;
    RotationRPY rotation;
    Mode mode;
    bool armed;
    bool connected;
    Battery battery;
    Frame frame;
    string ToString();
};


string frame_ToString(Frame frame);
string mode_ToString(Mode mode);
//functions
float get_dist(PointXYZ p1, PointXYZ p2);

MAV_FRAME frame_to_mav_frame(Frame frame);
Mode custom_mode_to_mode(uint32_t mav_mode);
Mode base_mode_to_mode(uint8_t mav_mode);
// bool base_mode_to_arm(uint8_t mav_mode);

PointXYZ enu_to_ned(PointXYZ enu);
PointXYZ ned_to_enu(PointXYZ ned);

PointXYZyaw enu_to_ned(PointXYZyaw enu);
PointXYZyaw ned_to_enu(PointXYZyaw ned);

std::string format (const char *fmt, ...);
std::string vformat (const char *fmt, va_list ap);
} // namespace mavlink_drone_sdk
#endif