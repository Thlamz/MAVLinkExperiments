#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/array.hpp>
#include <boost/asio/steady_timer.hpp>
#include "mavlink_helper.hpp"

#ifndef COMMAND_ROUTINES_HEADER
#define COMMAND_ROUTINES_HEADER
/**
 * @brief Coroutine designed to send commands encoded in MAVLink messages to a MAVProxy endpoint
 * 
 */
class MainCommandRoutine : public boost::asio::coroutine {
    public:
        /**
         * @brief Construct a new Main Command Routine object
         * 
         * @param mavlink_helper Instance of the MAVLinkHelper configured for communication with the MAVProxy endpoint
         */
        MainCommandRoutine(std::shared_ptr<MAVLinkHelper>& mavlink_helper) : helper(mavlink_helper) {
            command_timeout = new boost::asio::steady_timer(helper->io);
        };
        /**
         * @brief Operation to start the coroutine. It will send a sequence of commands to the MAVProxy endpoint that will arm the vehicle and command it
         * to takeoff to 25 meters.
         * 
         * @param yield Yield context of the coroutine
         */
        void operator()(boost::asio::yield_context yield);

        /**
         * @brief Asyncronously sends a message and sets a requirement with an optional timeout. Always waits for COMMAND_ACK
         * 
         * @param msg MAVLink message to send
         * @param requirement Requirement for the message's completion. This is a function that will be 
         * called with every oncoming telemetry to check for the requirement's completion
         * @param yield Yield context of the stackful coroutine
         * @param timeout Optional timeout in seconds. After this time the requirement will complete regardless of any telemetry recieved
         * @param retries Optional number of retries for the message
         * @return boost::system::error_code 
         */
        boost::system::error_code async_send_command(mavlink_command_long_t cmd, Condition requirement, boost::asio::yield_context yield, int timeout = -1, int retries = -1);

    protected:
        std::shared_ptr<MAVLinkHelper>& helper;
        size_t len;
        boost::array<uint8_t, 256> buf;
        boost::asio::steady_timer* command_timeout;
};


#endif