#include "command_routines.hpp"
#include "message_checkers.hpp"

using namespace boost::asio;
using boost::asio::ip::udp;

void CommandRoutine::operator()(boost::system::error_code ec, std::size_t n) {
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
            mavlink_msg_command_long_encode(helper.system_id, helper.component_id, &msg, &cmd);
            len = mavlink_msg_to_send_buffer(buf.data(), &msg);

            yield helper.socket.async_send_to(buffer(buf, len), helper.remote_port, *this);


            yield helper.add_requirement(new requirement(check_pre_arm), *this);
            
            

            std::cout << "Sending ARM THROTTLE" << std::endl;
            // Sending ARM THROTTLE command
            cmd = {};
            cmd.command = MAV_CMD_COMPONENT_ARM_DISARM;
            cmd.confirmation = 0;
            cmd.param1 = 1;
            cmd.target_component = 1;
            cmd.target_system = 1;
            mavlink_msg_command_long_encode(helper.system_id, helper.component_id, &msg, &cmd);
            len = mavlink_msg_to_send_buffer(buf.data(), &msg);

            yield helper.socket.async_send_to(buffer(buf, len), helper.remote_port, *this);
            yield helper.add_requirement(new requirement(check_arm), *this);


            std::cout << "Sending TAKEOFF" << std::endl;
            // Taking off to 25 meters
            cmd = {};
            cmd.command = MAV_CMD_NAV_TAKEOFF;
            cmd.confirmation = 0;
            cmd.param7 = 25;
            cmd.target_component = 1;
            cmd.target_system = 1;
            mavlink_msg_command_long_encode(helper.system_id, helper.component_id, &msg, &cmd);
            len = mavlink_msg_to_send_buffer(buf.data(), &msg);

            yield helper.socket.async_send_to(buffer(buf, len), helper.remote_port, *this);
        };
    }
}