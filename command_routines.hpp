#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/array.hpp>
#include "mavlink_helper.hpp"


#ifndef COMMAND_ROUTINES_HEADER
#define COMMAND_ROUTINES_HEADER

class MainCommandRoutine : public boost::asio::coroutine {
    public:
        MainCommandRoutine(std::shared_ptr<MAVLinkHelper>& mavlink_helper) : helper(mavlink_helper) {};
        void operator()(boost::asio::yield_context yield);

    private:
        std::shared_ptr<MAVLinkHelper>& helper;
        boost::array<uint8_t, 256> buf;
        size_t len;
};


// class SetupCommandRoutine : public boost::asio::coroutine {
//     public:
//         SetupCommandRoutine(std::shared_ptr<MAVLinkHelper>& mavlink_helper) : helper(mavlink_helper) {};
//         void operator()(boost::system::error_code ec = boost::system::error_code(), std::size_t n = 0);

//     private:
//         std::shared_ptr<MAVLinkHelper>& helper;
//         boost::array<uint8_t, 256> buf;
//         size_t len;
// };


#endif