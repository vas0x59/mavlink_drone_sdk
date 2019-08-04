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
MAV_FRAME frame_to_mav_frame(Frame frame)
{
    switch (frame)
    {
    case FRAME_BODY:
        /* code */
        return MAV_FRAME_LOCAL_NED;
        break;
    case FRAME_LOCAL:
        /* code */
        return MAV_FRAME_LOCAL_NED;
        break;
        
    case FRAME_VISION:
        /* code */
        return MAV_FRAME_VISION_NED;
        break;
    
    
    default:
        return MAV_FRAME_LOCAL_NED;
        break;
    }
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
    switch (mav_mode)
    {
    case 0:
        return MODE_MANUAL;
        break;
    case 1:
        return MODE_ALTITUDE;
        break;
    case 2:
        return MODE_POSITION;
        break;
    case 3:
        return MODE_MISSION;
        break;
    case 4:
        return MODE_HOLD;
        break;
    case 5:
        return MODE_RETURN;
        break;
    case 6:
        return MODE_ACRO;
        break;
    case 7:
        return MODE_OFFBOARD;
        break;
    case 8:
        return MODE_STABILIZE;
        break;
    case 9:
        return MODE_RATTITUDE;
        break;
    case 10:
        return MODE_TAKEOFF;
        break;
    case 11:
        return MODE_LAND;
        break;
    case 12:
        return MODE_FOLLOW_ME;
        break;
    }
    return mode;
}
Mode base_mode_to_mode(uint8_t mav_mode)
{
    return MODE_UNASSIGNED;
}

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

// tostring
string PointXYZ::ToString(){
    return "[" + to_string(x) + " " + to_string(y) + " " + to_string(z) + "]";
}
string RotationRPY::ToString(){
    return "[" + to_string(roll) + " " + to_string(pitch) + " " + to_string(yaw) + "]";
}
string PointXYZyaw::ToString(){
    return "[" + to_string(x) + " " + to_string(y) + " " + to_string(z) + " " + to_string(yaw)  + "]";
}

string mode_ToString(Mode mode)
{
    // Mode mode;
    switch (mode)
    {
    case MODE_MANUAL:
        return "MODE_MANUAL";
        break;
    case MODE_ALTITUDE:
        return "MODE_ALTITUDE";
        break;
    case MODE_POSITION:
        return "MODE_POSITION";
        break;
    case MODE_MISSION:
        return "MODE_MISSION";
        break;
    case MODE_HOLD:
        return "MODE_HOLD";
        break;
    case MODE_RETURN:
        return "MODE_RETURN";
        break;
    case MODE_ACRO:
        return "MODE_ACRO";
        break;
    case MODE_OFFBOARD:
        return "MODE_OFFBOARD";
        break;
    case MODE_STABILIZE:
        return "MODE_STABILIZE";
        break;
    case MODE_RATTITUDE:
        return "MODE_RATTITUDE";
        break;
    case MODE_TAKEOFF:
        return "MODE_TAKEOFF";
        break;
    case MODE_LAND:
        return "MODE_LAND";
        break;
    case MODE_FOLLOW_ME:
        return "MODE_FOLLOW_ME";
        break;
    case MODE_UNASSIGNED:
        return "MODE_UNASSIGNED";
        break;
    }
    return "MODE_UNASSIGNED";
    // return mode;
}

string Telemetry::ToString(){
    // return
    string str_out = "Telemetry: \n";
    str_out += " Position: " + position.ToString() + "\n";
    str_out += " Velocity: " + velocity.ToString() + "\n";
    str_out += " Rotation: " + rotation.ToString() + "\n";
    str_out += " Mode: "     + mode_ToString(mode)  + "\n";
    str_out += " Armed: "    + to_string(armed)    + "\n";
    str_out += " Connected: "+ to_string(connected)+ "\n";
    return str_out;
}

inline double hypot(double x, double y, double z)
{
	return std::sqrt(x * x + y * y + z * z);
}


float get_dist(PointXYZ p1, PointXYZ p2){

	return hypot(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);

}

// enu ned

PointXYZ enu_to_ned(PointXYZ enu){
    PointXYZ ned;
    ned.x = enu.y;
    ned.y = enu.x;
    ned.z = -enu.z;
    return ned;
};
PointXYZ ned_to_enu(PointXYZ ned){
    PointXYZ enu;
    enu.x = ned.y;
    enu.y = ned.x;
    enu.z = -ned.z;
    return enu;
};

PointXYZyaw enu_to_ned(PointXYZyaw enu){
    PointXYZyaw ned;
    ned.x = enu.y;
    ned.y = enu.x;
    ned.z = -enu.z;
    ned.yaw = enu.yaw;
    return ned;
};
PointXYZyaw ned_to_enu(PointXYZyaw ned){
    PointXYZyaw enu;
    enu.x = ned.y;
    enu.y = ned.x;
    enu.z = -ned.z;
    enu.yaw = ned.yaw;
    return enu;
};
// float get_dist
}; // namespace mavlink_indoor_sdk