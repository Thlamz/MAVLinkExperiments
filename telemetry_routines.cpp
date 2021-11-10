#include "telemetry_routines.hpp"

using namespace boost::asio;
using boost::asio::ip::udp;

void TelemetryRoutine::operator()(boost::system::error_code ec, std::size_t len) {
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
                            helper->check_requirement(msg);
                        }
                    }
            }
        };
    }
}