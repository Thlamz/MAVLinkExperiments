#include "telemetry_routines.hpp"
#include <iostream>

using namespace boost::asio;
using boost::asio::ip::udp;

void TelemetryRoutine::operator()(boost::asio::yield_context yield) {
    size_t len;
    for(;;) {
        len = helper->socket.async_receive(buffer(*buf), yield);

        mavlink_status_t status;
        mavlink_message_t msg;

        for (int i = 0; i < len; ++i)
        {
            if (mavlink_parse_char(MAVLINK_COMM_0, (*buf)[i], &msg, &status))
            {
                helper->check_requirement(msg);
            }
        }
    }
}