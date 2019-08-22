#include "drone.h"
namespace mavlink_indoor_sdk
{
Drone::Drone(autopilot_interface::AutopilotInterface *ai_)
{
    ai = ai_;
    // ai.start
}

void Drone::start()
{
    ai->start();
    usleep(100);
    ai->enable_offboard_control();
    usleep(100);
}
void Drone::stop()
{
    ai->disable_offboard_control();
    ai->stop();
}
void Drone::handle_quit(int sig){
    
    ai->handle_quit(sig);
}

Drone::~Drone()
{
    stop();
}

void Drone::navigate(PointXYZyaw pose, Frame frame, float speed)
{
    if (log >= 1)
    {
        LogInfo("Drone", "Navigate pose:" + pose.ToString() + " frame:" + frame_ToString(frame) + " speed:" + to_string(speed));
    }
    mavlink_set_position_target_local_ned_t setpoint;
    MAV_FRAME mav_frame = frame_to_mav_frame(frame);
    pose = enu_to_ned(pose);
    // pose = enu_to_ned(pose);
    autopilot_interface::set_position(pose.x, pose.y, pose.z, mav_frame, setpoint);
    autopilot_interface::set_yaw(pose.yaw, setpoint);

    ai->update_nav_setpoint(setpoint, speed);
}
void Drone::set_position(PointXYZyaw pose, Frame frame)
{
    if (log >= 1)
    {
        LogInfo("Drone", "Set_Position pose:" + pose.ToString() + " frame:" + frame_ToString(frame));
    }
    mavlink_set_position_target_local_ned_t setpoint;
    MAV_FRAME mav_frame = frame_to_mav_frame(frame);
    pose = enu_to_ned(pose);
    // cout << pose.ToString();
    autopilot_interface::set_position(pose.x, pose.y, pose.z, mav_frame, setpoint);
    autopilot_interface::set_yaw(pose.yaw, setpoint);

    ai->update_setpoint(setpoint);
}
Telemetry Drone::get_telemetry(Frame frame)
{
    // MAV_FRAME mav_frame = frame_to_mav_frame(frame);
    Telemetry telemetry;
    telemetry.frame = frame;
    if (frame == FRAME_VISION)
    {
        telemetry.position.x = ai->current_messages.local_position_vision_ned.x;
        telemetry.position.y = ai->current_messages.local_position_vision_ned.y;
        telemetry.position.z = ai->current_messages.local_position_vision_ned.z;

        telemetry.velocity.x = ai->current_messages.local_position_vision_ned.vx;
        telemetry.velocity.y = ai->current_messages.local_position_vision_ned.vy;
        telemetry.velocity.z = ai->current_messages.local_position_vision_ned.vz;
    }
    else
    {
        telemetry.position.x = ai->current_messages.local_position_ned.x;
        telemetry.position.y = ai->current_messages.local_position_ned.y;
        telemetry.position.z = ai->current_messages.local_position_ned.z;

        telemetry.velocity.x = ai->current_messages.local_position_ned.vx;
        telemetry.velocity.y = ai->current_messages.local_position_ned.vy;
        telemetry.velocity.z = ai->current_messages.local_position_ned.vz;
    }
    telemetry.position = ned_to_enu(telemetry.position);
    telemetry.velocity = ned_to_enu(telemetry.velocity);

    telemetry.rotation.roll = ai->current_messages.attitude.roll;
    telemetry.rotation.pitch = ai->current_messages.attitude.pitch;
    telemetry.rotation.yaw = ai->current_messages.attitude.yaw;

    telemetry.connected = ai->connected;

    telemetry.mode = custom_mode_to_mode(ai->current_messages.heartbeat.custom_mode);
    // telemetry.mode = base_mode_to_mode(ai->current_messages.heartbeat.base_mode);

    // telemetry.armed = base_mode_to_arm(ai->current_messages.heartbeat.base_mode);
    telemetry.armed = ((ai->current_messages.heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED) ? true : false);
    // telemetry.battery.cells_count = ai->current_messages.battery_status
    return telemetry;
    // for
}

void Drone::toggle_arming(bool arm_)
{
    // MAV_CMD_COMPONENT_ARM_DISARM
    // mavlink_comm
    mavlink_command_long_t com = {0};
    com.target_system = 1;
    com.target_component = MAV_COMP_ID_ALL;
    cout << "sys: " << ai->system_id << "auto: " << MAV_COMP_ID_ALL << "\n";
    com.command = MAV_CMD_COMPONENT_ARM_DISARM;
    // com.confirmation = true;
    com.param1 = arm_; // flag >0.5 => start, <0.5 => stop
    // Encode
    mavlink_message_t message;
    mavlink_msg_command_long_encode(ai->system_id, MAV_COMP_ID_ALL, &message, &com);

    // Send the message
    int len = ai->write_message(message);
}
void Drone::arm()
{
    LogWarn("Drone", "ARM");
    toggle_arming(true);
}

void Drone::disarm()
{
    LogWarn("Drone", "DISARM");
    toggle_arming(false);
}

void Drone::takeoff(float alt, float speed)
{
    arm();
    sleep(1000);
    // navigate({0, 0, alt, 0}, FRAME_BODY, speed);
    set_position({0, 0, 0, 0}, FRAME_BODY);
    // navigate({0, 0, alt, 0}, FRAME_BODY, 3);
    sleep(1000);
    navigate({0, 0, alt, 0}, FRAME_LOCAL, speed);
    sleep(3000);

}
void Drone::land()
{
    LogWarn("Drone", "LAND");
    // MAV_CMD_NAV_LAND_LOCAL
    mavlink_command_long_t com = {0};
    com.target_system = 1;
    com.target_component = MAV_COMP_ID_ALL;
    com.command = MAV_CMD_NAV_LAND;
    com.confirmation = true;
    com.param1 = NAN;
    com.param2 = NAN;
    com.param3 = NAN;
    com.param4 = NAN;
    com.param5 = NAN;
    com.param6 = NAN;
    com.param7 = NAN;
    // com.param1 = (float)arm_; // flag >0.5 => start, <0.5 => stop
    // Encode
    mavlink_message_t message;
    mavlink_msg_command_long_encode(ai->system_id, MAV_COMP_ID_ALL, &message, &com);

    // Send the message
    int len = ai->write_message(message);

    sleep(5000);
    // disarm();
}
void Drone::sleep(uint16_t msec)
{
    if (log >= 1)
    {
        LogInfo("Drone", "Sleep: " + to_string(msec));
    }
    usleep((uint32_t)msec * 1000);
};
void Drone::navigate_wait(PointXYZyaw pose, Frame frame, float speed, float thresh)
{
    navigate(pose, frame, speed);
    // to_string()
    uint8_t j = 1;
    while (true)
    {
        Telemetry telemetry = get_telemetry(frame);
        // std::cout << telemetry.ToString() << std::endl;
        if (log == 1)
        {
            if (j == ((int)300 / (int)80))
            {
                LogInfo("Drone", "Telemetry: \n" + telemetry.ToString());
                j = 0;
            }
        }
        if (log == 2)
        {
            LogInfo("Drone", "Telemetry: \n" + telemetry.ToString());
        }
        if (get_dist(telemetry.position, {pose.x, pose.y, pose.z}) < thresh)
        {
            break;
        }

        usleep(80 * 1000);
        j++;
        // cout << "z: " <<  telemetry.position.x << "y: " << telemetry.position.y << "z: " << telemetry.position.z << endl;
    }
}
}; // namespace mavlink_indoor_sdk