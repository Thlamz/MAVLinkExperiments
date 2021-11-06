#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <ardupilotmega/mavlink.h>

#ifndef MAVLINKHELPER_HEADER
#define MAVLINKHELPER_HEADER

typedef std::function<bool(mavlink_message_t)> Condition;

class MAVLinkHelper {
    public:
        MAVLinkHelper(uint8_t system_id, uint8_t component_id, int input_port, int output_port);
        ~MAVLinkHelper();

        void add_requirement(Condition cond, std::function<void()> resume);

        void check_requirements(mavlink_message_t msg);

    public:
        uint8_t system_id;
        uint8_t component_id;
        boost::asio::io_context io;
        boost::asio::ip::udp::resolver resolver = boost::asio::ip::udp::resolver(io);
        boost::asio::ip::udp::socket socket = boost::asio::ip::udp::socket(io);
        boost::asio::ip::udp::endpoint local_port;
        boost::asio::ip::udp::endpoint remote_port;

    
    private:
        boost::array<uint8_t, 265> buf;
        
        Condition current_condition;
        std::function<void()> completer;
        bool current_condition_resolved = false;
};

#endif