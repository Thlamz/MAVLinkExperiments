#include <iostream>
#include <queue>
#include <ardupilotmega/mavlink.h>
#include <boost/asio.hpp>
#include <boost/asio/yield.hpp>
#include <boost/array.hpp>
#include <boost/stacktrace.hpp>

#include "message_checkers.hpp"
#include "mavlink_helper.hpp"
#include "command_routines.hpp"
#include "telemetry_routines.hpp"

using namespace boost::asio;
using boost::asio::ip::udp;

int main(void) {
    try {
        std::shared_ptr<MAVLinkHelper> helper = std::make_shared<MAVLinkHelper>(203, 200, 5605, 5505);

        TelemetryRoutine telemetry_routine(helper);
        telemetry_routine();

        CommandRoutine command_routine(helper);
        command_routine();

        helper->io.run();
    } catch(std::exception &error) {
        std::cout << "Error: " << error.what() << std::endl;
        std::cout << boost::stacktrace::stacktrace() << std::endl;
    }

    return 0;
}