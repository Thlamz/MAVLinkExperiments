#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <ardupilotmega/mavlink.h>
#include "requirement.hpp"

using namespace boost::asio;
using boost::asio::ip::udp;

class MAVLinkHelper {
    public:
        MAVLinkHelper(uint8_t system_id, uint8_t component_id, int input_port, int output_port);
        ~MAVLinkHelper();

        void add_requirement(requirement *req, std::function<void()> resume);

        void check_requirements(mavlink_message_t msg);

    public:
        uint8_t system_id;
        uint8_t component_id;
        io_context io;
        udp::resolver resolver = udp::resolver(io);
        udp::socket socket = udp::socket(io);
        udp::endpoint local_port;
        udp::endpoint remote_port;

    
    private:
        boost::array<uint8_t, 265> buf;
        
        requirement *current_requirement;
        std::function<void()> completer;
};