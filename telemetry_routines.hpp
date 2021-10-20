#include <boost/asio.hpp>
#include <boost/asio/yield.hpp>
#include <boost/array.hpp>
#include "mavlink_helper.hpp"


#ifndef TELEMETRY_ROUTINES_HEADER
#define TELEMETRY_ROUTINES_HEADER

class TelemetryRoutine : public boost::asio::coroutine {
    public:
        TelemetryRoutine(std::shared_ptr<MAVLinkHelper>& mavlink_helper) : helper(mavlink_helper) {}; 
        void operator()(boost::system::error_code ec = boost::system::error_code(), std::size_t len = 0);
    private:
        std::shared_ptr<MAVLinkHelper>& helper;
        boost::array<uint8_t, 256> *buf = new boost::array<uint8_t, 256>();
};

#endif