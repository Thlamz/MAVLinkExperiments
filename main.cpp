#include <iostream>
#include <queue>
#include <ardupilotmega/mavlink.h>
#include <boost/asio.hpp>
#include <boost/asio/yield.hpp>
#include <boost/array.hpp>
#include <boost/stacktrace.hpp>

#include "message_checkers.hpp"
#include "mavlink_helper.hpp"

using namespace boost::asio;
using boost::asio::ip::udp;

struct telemetry_routine : coroutine {
    std::shared_ptr<MAVLinkHelper> helper;
    boost::array<uint8_t, 2048> *buf = new boost::array<uint8_t, 2048>();

    telemetry_routine(std::shared_ptr<MAVLinkHelper> mavlink_helper) {
        helper = mavlink_helper;
    };

    void operator()(boost::system::error_code ec = boost::system::error_code(), std::size_t len = 0) {
        if(!ec) {
            reenter(this) {
                for(;;) {
                        yield helper->socket.async_receive(buffer(*buf), *this);

                        mavlink_status_t status;
                        mavlink_message_t msg;

                        for (int i = 0; i < len; ++i)
                        {
                            if (mavlink_parse_char(MAVLINK_COMM_0, (*buf)[i], &msg, &status))
                            {
                                helper->check_requirements(msg);
                            }
                        }
                }
            };
        }
    }
};


struct command_routine : coroutine {
    std::shared_ptr<MAVLinkHelper> helper;
    boost::array<uint8_t, 256> buf;
    size_t len;

    command_routine(std::shared_ptr<MAVLinkHelper> mavlink_helper) {
        helper = mavlink_helper;
    };

    void operator()(boost::system::error_code ec = boost::system::error_code(), std::size_t n = 0) {
        if(!ec) {
            reenter(this) {
                std::cout << "Sending GUIDED" << std::endl;
                // Sending MODE GUIDED command
                mavlink_command_long_t cmd;
                cmd.command = MAV_CMD_DO_SET_MODE;
                cmd.confirmation = 0;
                cmd.param1 = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
                cmd.param2 = COPTER_MODE_GUIDED;
                cmd.target_component = 1;
                cmd.target_system = 1;
                mavlink_message_t msg; 
                mavlink_msg_command_long_encode(helper->system_id, helper->component_id, &msg, &cmd);
                len = mavlink_msg_to_send_buffer(buf.data(), &msg);

                yield helper->socket.async_send_to(buffer(buf, len), helper->remote_port, *this);

  
                yield helper->add_requirement(new requirement(check_pre_arm), *this);
                
                

                std::cout << "Sending ARM THROTTLE" << std::endl;
                // Sending ARM THROTTLE command
                cmd = {};
                cmd.command = MAV_CMD_COMPONENT_ARM_DISARM;
                cmd.confirmation = 0;
                cmd.param1 = 1;
                cmd.target_component = 1;
                cmd.target_system = 1;
                mavlink_msg_command_long_encode(helper->system_id, helper->component_id, &msg, &cmd);
                len = mavlink_msg_to_send_buffer(buf.data(), &msg);

                yield helper->socket.async_send_to(buffer(buf, len), helper->remote_port, *this);
                yield helper->add_requirement(new requirement(check_arm), *this);


                std::cout << "Sending TAKEOFF" << std::endl;
                // Taking off to 25 meters
                cmd = {};
                cmd.command = MAV_CMD_NAV_TAKEOFF;
                cmd.confirmation = 0;
                cmd.param7 = 25;
                cmd.target_component = 1;
                cmd.target_system = 1;
                mavlink_msg_command_long_encode(helper->system_id, helper->component_id, &msg, &cmd);
                len = mavlink_msg_to_send_buffer(buf.data(), &msg);

                yield helper->socket.async_send_to(buffer(buf, len), helper->remote_port, *this);
            };
        }
    }
};

int main(void) {
    try {
        std::shared_ptr<MAVLinkHelper> helper = std::make_shared<MAVLinkHelper>(203, 200, 5605, 5505);

        telemetry_routine telemetry_routine(helper);
        telemetry_routine();

        command_routine command_routine(helper);
        command_routine();

        helper->io.run();
    } catch(std::exception &error) {
        std::cout << "Error: " << error.what() << std::endl;
        std::cout << boost::stacktrace::stacktrace() << std::endl;
    }

    return 0;
}