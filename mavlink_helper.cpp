#include "mavlink_helper.hpp"
#include <iostream>

using namespace boost::asio;
using boost::asio::ip::udp;

MAVLinkHelper::MAVLinkHelper(uint8_t system_id, uint8_t component_id, int port) : system_id(system_id), component_id(component_id) {
    socket.open(udp::v4());
    local_port = udp::endpoint(udp::v4(), port);
    socket.bind(local_port);

    // Resolving remote port
    std::cout << "Resolving port" << std::endl;
    boost::array<uint8_t, 256> array;
    socket.receive_from(buffer(array), remote_port);
    socket.connect(remote_port);
}
MAVLinkHelper::~MAVLinkHelper() {
    socket.close();
}

void MAVLinkHelper::add_requirement(Condition cond, std::function<void()> resume) {
    current_condition = cond;
    completer = resume;
    current_condition_resolved = false;
}

void MAVLinkHelper::check_requirement(mavlink_message_t msg) {
    if(!current_condition_resolved && current_condition(msg)) {
        current_condition_resolved = true;
        completer();
    }
}

void MAVLinkHelper::cancel_requirement() {
    if(!current_condition_resolved) {
        current_condition_resolved = true;
    }
}