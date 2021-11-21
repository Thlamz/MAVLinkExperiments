#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/asio/spawn.hpp>
#include "mavlink_helper.hpp"


#ifndef TELEMETRY_ROUTINES_HEADER
#define TELEMETRY_ROUTINES_HEADER

/**
 * @brief Coroutine designed to received messages from a MAVProxy port and decode it as MAVLink messages
 * 
 */
class TelemetryRoutine : public boost::asio::coroutine {
    public:
        /**
         * @brief Construct a new Telemetry Routine object
         * 
         * @param mavlink_helper Instance of the MAVLinkHelper configured for communication with the MAVProxy endpoint
         */
        TelemetryRoutine(std::shared_ptr<MAVLinkHelper>& mavlink_helper) : helper(mavlink_helper) {}; 
        /**
         * @brief Operation to start the coroutine. Constantly received information from the MAVProxy endpoint and decodes it as MAVLink messages.
         * The messages get sent to the MAVLinkHelper to check requirements by the check_requirements function.
         * 
         * @param yield Yield context of the coroutine
         */
        void operator()(boost::asio::yield_context yield);
    private:
        std::shared_ptr<MAVLinkHelper>& helper;
        boost::array<uint8_t, 256> *buf = new boost::array<uint8_t, 256>();
};

#endif