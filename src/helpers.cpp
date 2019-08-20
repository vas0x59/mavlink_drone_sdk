#include "helpers.h"

using namespace std;

namespace mavlink_indoor_sdk
{
uint64_t get_time_usec()
{
    static struct timeval _time_stamp;
    gettimeofday(&_time_stamp, NULL);
    return _time_stamp.tv_sec * 1000000 + _time_stamp.tv_usec;
}
uint64_t get_time_msec()
{
    static struct timeval _time_stamp;
    gettimeofday(&_time_stamp, NULL);
    return _time_stamp.tv_sec * 1000 + _time_stamp.tv_usec/1000;
}
MAV_FRAME frame_to_mav_frame(Frame frame)
{
    switch (frame)
    {
    case FRAME_BODY:
        return MAV_FRAME_LOCAL_NED;
    case FRAME_LOCAL:
        return MAV_FRAME_LOCAL_NED;
    case FRAME_VISION:
        return MAV_FRAME_VISION_NED;
    }
    return MAV_FRAME_LOCAL_NED;
}
/*

    -1: Unassigned
    0: Manual
    1: Altitude
    2: Position
    3: Mission
    4: Hold
    5: Return
    6: Acro
    7: Offboard
    8: Stabilized
    9: Rattitude
    10: Takeoff
    11: Land
    12: Follow Me

 */
Mode custom_mode_to_mode(uint32_t mav_mode)
{
    Mode mode;
    px4::px4_custom_mode px4_mode;
    px4_mode.data = mav_mode;

    switch (px4_mode.main_mode)
    {
    case px4::PX4_CUSTOM_MAIN_MODE_MANUAL:
        return MODE_MANUAL;
    case px4::PX4_CUSTOM_MAIN_MODE_ALTCTL:
        return MODE_ALTITUDE;
    case px4::PX4_CUSTOM_MAIN_MODE_POSCTL:
        return MODE_POSITION;
    case px4::PX4_CUSTOM_MAIN_MODE_ACRO:
        return MODE_ACRO;
    case px4::PX4_CUSTOM_MAIN_MODE_OFFBOARD:
        return MODE_OFFBOARD;
    case px4::PX4_CUSTOM_MAIN_MODE_STABILIZED:
        return MODE_STABILIZE;
    case px4::PX4_CUSTOM_MAIN_MODE_RATTITUDE:
        return MODE_RATTITUDE;
    }
    switch (px4_mode.sub_mode)
    {
    case px4::PX4_CUSTOM_SUB_MODE_AUTO_MISSION:
        return MODE_MISSION;
    case px4::PX4_CUSTOM_SUB_MODE_AUTO_LOITER:
        return MODE_HOLD;
    case px4::PX4_CUSTOM_SUB_MODE_AUTO_RTL:
        return MODE_RETURN;
    case px4::PX4_CUSTOM_SUB_MODE_AUTO_TAKEOFF:
        return MODE_TAKEOFF;
    case px4::PX4_CUSTOM_SUB_MODE_AUTO_LAND:
        return MODE_LAND;
    }
    return MODE_UNASSIGNED;
    // return mode;
}
Mode base_mode_to_mode(uint8_t mav_mode)
{
    return MODE_UNASSIGNED;
}

/*
bool base_mode_to_arm(uint8_t mav_mode)
{
    bool armed = false;
    if (
        (mav_mode == MAV_MODE_STABILIZE_DISARMED) &&
        (mav_mode == MAV_MODE_MANUAL_DISARMED) &&
        (mav_mode == MAV_MODE_GUIDED_DISARMED) &&
        (mav_mode == MAV_MODE_AUTO_DISARMED) &&
        (mav_mode == MAV_MODE_TEST_DISARMED))
    {
        armed = false;
    }
    else if (
        (mav_mode == MAV_MODE_STABILIZE_ARMED) &&
        (mav_mode == MAV_MODE_MANUAL_ARMED) &&
        (mav_mode == MAV_MODE_GUIDED_ARMED) &&
        (mav_mode == MAV_MODE_AUTO_ARMED) &&
        (mav_mode == MAV_MODE_TEST_ARMED))
    {
        armed = true;
    }
    return armed;
}
*/

// tostring
string PointXYZ::ToString()
{
    return "[" + to_string(x) + " " + to_string(y) + " " + to_string(z) + "]";
}
string RotationRPY::ToString()
{
    return "roll:" + to_string(roll) + " pitch:" + to_string(pitch) + " yaw:" + to_string(yaw);
}
string PointXYZyaw::ToString()
{
    return "[" + to_string(x) + " " + to_string(y) + " " + to_string(z) + " " + to_string(yaw) + "]";
}

string frame_ToString(Frame frame)
{
    switch (frame)
    {
        case FRAME_LOCAL:
            return "FRAME_LOCAL";
        case FRAME_BODY:
            return "FRAME_BODY";   
        case FRAME_VISION:
            return "FRAME_VISION";
    }
    return "FRAME_LOCAL";
}

string mode_ToString(Mode mode)
{
    // Mode mode;
    switch (mode)
    {
    case MODE_MANUAL:
        return "MODE_MANUAL";
    case MODE_ALTITUDE:
        return "MODE_ALTITUDE";
    case MODE_POSITION:
        return "MODE_POSITION";
    case MODE_MISSION:
        return "MODE_MISSION";
    case MODE_HOLD:
        return "MODE_HOLD";
    case MODE_RETURN:
        return "MODE_RETURN";
    case MODE_ACRO:
        return "MODE_ACRO";
    case MODE_OFFBOARD:
        return "MODE_OFFBOARD";
    case MODE_STABILIZE:
        return "MODE_STABILIZE";
    case MODE_RATTITUDE:
        return "MODE_RATTITUDE";
    case MODE_TAKEOFF:
        return "MODE_TAKEOFF";
    case MODE_LAND:
        return "MODE_LAND";
    case MODE_FOLLOW_ME:
        return "MODE_FOLLOW_ME";
    case MODE_UNASSIGNED:
        return "MODE_UNASSIGNED";
    }
    return "MODE_UNASSIGNED";
    // return mode;
}

string Telemetry::ToString()
{
    // return
    string str_out = "Telemetry: \n";
    str_out += " Position: " + position.ToString() + "\n";
    str_out += " Velocity: " + velocity.ToString() + "\n";
    str_out += " Rotation: " + rotation.ToString() + "\n";
    str_out += " Mode: " + mode_ToString(mode) + "\n";
    str_out += " Armed: " + to_string(armed) + "\n";
    str_out += " Connected: " + to_string(connected) + "\n";
    return str_out;
}

inline double hypot(double x, double y, double z)
{
    return std::sqrt(x * x + y * y + z * z);
}

float get_dist(PointXYZ p1, PointXYZ p2)
{

    return hypot(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}

// enu ned

PointXYZ enu_to_ned(PointXYZ enu)
{
    PointXYZ ned;
    ned.x = enu.y;
    ned.y = enu.x;
    ned.z = -enu.z;
    return ned;
};
PointXYZ ned_to_enu(PointXYZ ned)
{
    PointXYZ enu;
    enu.x = ned.y;
    enu.y = ned.x;
    enu.z = -ned.z;
    return enu;
};

PointXYZyaw enu_to_ned(PointXYZyaw enu)
{
    PointXYZyaw ned;
    ned.x = enu.y;
    ned.y = enu.x;
    ned.z = -enu.z;
    ned.yaw = enu.yaw;
    return ned;
};
PointXYZyaw ned_to_enu(PointXYZyaw ned)
{
    PointXYZyaw enu;
    enu.x = ned.y;
    enu.y = ned.x;
    enu.z = -ned.z;
    enu.yaw = ned.yaw;
    return enu;
};

std::string format (const char *fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    std::string buf = vformat (fmt, ap);
    va_end (ap);
    return buf;
}


std::string vformat (const char *fmt, va_list ap)
{
    // Allocate a buffer on the stack that's big enough for us almost
    // all the time.
    size_t size = 1024;
    char buf[size];

    // Try to vsnprintf into our buffer.
    va_list apcopy;
    va_copy (apcopy, ap);
    int needed = vsnprintf (&buf[0], size, fmt, ap);
    // NB. On Windows, vsnprintf returns -1 if the string didn't fit the
    // buffer.  On Linux & OSX, it returns the length it would have needed.

    if (needed <= size && needed >= 0) {
        // It fit fine the first time, we're done.
        return std::string (&buf[0]);
    } else {
        // vsnprintf reported that it wanted to write more characters
        // than we allotted.  So do a malloc of the right size and try again.
        // This doesn't happen very often if we chose our initial size
        // well.
        std::vector <char> buf;
        size = needed;
        buf.resize (size);
        needed = vsnprintf (&buf[0], size, fmt, apcopy);
        return std::string (&buf[0]);
    }
}
// float get_dist
}; // namespace mavlink_indoor_sdk