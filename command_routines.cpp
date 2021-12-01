#include "command_routines.hpp"
#include "message_checkers.hpp"
#include "requirement.hpp"
#include <iostream>

using namespace boost::asio;
using boost::asio::ip::udp;

void MainCommandRoutine::operator()(boost::asio::yield_context yield) {
    boost::system::error_code ec;
    mavlink_command_long_t cmd;

    std::cout << "Setting STREAM RATE" << std::endl;
    cmd = {};
    cmd.command = MAV_CMD_SET_MESSAGE_INTERVAL;
    cmd.confirmation = 0;
    cmd.param1 = MAVLINK_MSG_ID_HEARTBEAT;
    cmd.param2 = 10; // 10 Hz
    cmd.param7 = 0;
    cmd.target_component = 1;
    cmd.target_system = 1;
    async_send_command(cmd, check_empty, yield, 15, 3);

    std::cout << "Sending GUIDED" << std::endl;
    // Sending MODE GUIDED command
    cmd = {};
    cmd.command = MAV_CMD_DO_SET_MODE;
    cmd.confirmation = 0;
    cmd.param1 = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
    cmd.param2 = COPTER_MODE_GUIDED;
    cmd.target_component = 1;
    cmd.target_system = 1;
    async_send_command(cmd, check_pre_arm, yield, 15, 3);
    
    std::cout << "Sending ARM THROTTLE" << std::endl;
    // Sending ARM THROTTLE command
    cmd = {};
    cmd.command = MAV_CMD_COMPONENT_ARM_DISARM;
    cmd.confirmation = 0;
    cmd.param1 = 1;
    cmd.target_component = 1;
    cmd.target_system = 1;
    async_send_command(cmd, check_arm, yield, 15, 3);



    std::cout << "Sending TAKEOFF" << std::endl;
    // Taking off to 25 meters
    cmd = {};
    cmd.command = MAV_CMD_NAV_TAKEOFF;
    cmd.confirmation = 0;
    cmd.param7 = 25;
    cmd.target_component = 1;
    cmd.target_system = 1;
    async_send_command(cmd, get_check_altitude(25, 2), yield, 30);

    std::cout << "Command Routine DONE" << std::endl;
}

inline boost::system::error_code MainCommandRoutine::async_send_command(mavlink_command_long_t cmd, Condition requirement, boost::asio::yield_context yield, 
                                                                            int timeout, int retries) {
    boost::system::error_code ec;
    mavlink_message_t msg;

    mavlink_msg_command_long_encode(helper->system_id, helper->component_id, &msg, &cmd);

    int tries = 0;
    do {
        ec.clear();
        tries++;

        if(tries > 1) {
            std::cout << "RETRYING" << std::endl;
        }

        len = mavlink_msg_to_send_buffer(buf.data(), &msg);
        len = helper->socket.async_send(buffer(buf, len), yield);
        if(ec) {
            std::cout << "REQUEST FAILED" << std::endl;
            continue;
        }

        Condition check_ack = get_check_cmd_ack(cmd.command);

        bool ack_checked = false;
        bool requirement_checked = false;
        async_requirement([&](mavlink_message_t msg) {
            if(check_ack(msg)) {
                ack_checked = true;
            }
            if(requirement(msg)) {
                requirement_checked = true;
            }
            return ack_checked && requirement_checked;
        }, helper, yield[ec], timeout, command_timeout);
        command_timeout->cancel();
        
        if(ec) {
            std::cout << "TIMED OUT" << std::endl;
        }
    } while(ec && tries < retries);
    return ec;
}