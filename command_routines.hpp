#include <boost/asio.hpp>
#include <boost/asio/yield.hpp>
#include <boost/array.hpp>
#include "mavlink_helper.hpp"


#ifndef COMMAND_ROUTINES_HEADER
#define COMMAND_ROUTINES_HEADER

class CommandRoutine : public boost::asio::coroutine {
    public:
        CommandRoutine(MAVLinkHelper& mavlink_helper) : helper(mavlink_helper) {};
        void operator()(boost::system::error_code ec = boost::system::error_code(), std::size_t n = 0);

    private:
        MAVLinkHelper &helper;
        boost::array<uint8_t, 256> buf;
        size_t len;
        
};

#endif