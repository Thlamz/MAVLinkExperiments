#include "mavlink_helper.hpp"


MAVLinkHelper::MAVLinkHelper(uint8_t system_id, uint8_t component_id, int input_port, int output_port) : system_id(system_id), component_id(component_id) {
    socket.open(udp::v4());
    local_port = udp::endpoint(udp::v4(), input_port);
    socket.bind(local_port);
    remote_port = *resolver.resolve(udp::v4(), "127.0.0.1", std::to_string(output_port)).begin();
}
MAVLinkHelper::~MAVLinkHelper() {
    socket.close();
}

void MAVLinkHelper::check_requirements(mavlink_message_t msg) {
    if(current_requirement->condition(msg)) {
        completer();
    }
}


void MAVLinkHelper::add_requirement(requirement *req, std::function<void()> resume) {
    current_requirement = req;
    completer = resume;
}