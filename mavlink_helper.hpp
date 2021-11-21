#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <ardupilotmega/mavlink.h>

#ifndef MAVLINKHELPER_HEADER
#define MAVLINKHELPER_HEADER

typedef std::function<bool(mavlink_message_t)> Condition;

/**
 * @brief Helper class that will facilitate connection to MAVProxy
 * 
 */
class MAVLinkHelper {
    public:
        /**
         * @brief Construct a new MAVLinkHelper object
         * 
         * @param system_id ID of the current system. Ground control software usually use the upper part of the IDS (ex: 255).
         * @param component_id Component ids are identified in the MAV_COMPONENT enum. MAV_COMP_ID_MISSIONPLANNER should generally be used.
         * @param port Target port. MAVProxy automatically registers the ports 14550 and 14551 so these should generally be used, although others can be specified
         * using the --out option when launching MAVProxy
         */
        MAVLinkHelper(uint8_t system_id, uint8_t component_id, int port);
        /**
         * @brief Destroy the MAVLinkHelper object and close all connections
         * 
         */
        ~MAVLinkHelper();

        /**
         * @brief Adds a requirement. Requirements are better explained in the requirements.hpp modules.
         * 
         * @param cond Condition that should resolve the requirement
         * @param resume Function called then the requirement is resolved
         */
        void add_requirement(Condition cond, std::function<void()> resume);

        /**
         * @brief Checks if a requirement, if any are registered, is satisfied by msg. Should be called when new telemetry is received.
         * 
         * @param msg Telemetry received
         */
        void check_requirement(mavlink_message_t msg);

        /**
         * @brief Cancels the current requirement, if any are registered.
         * 
         */
        void cancel_requirement();

    public:
        uint8_t system_id;
        uint8_t component_id;
        boost::asio::io_context io;
        boost::asio::ip::udp::resolver resolver = boost::asio::ip::udp::resolver(io);
        boost::asio::ip::udp::socket socket = boost::asio::ip::udp::socket(io);
        boost::asio::ip::udp::endpoint local_port;
        boost::asio::ip::udp::endpoint remote_port;

    
    private:
        boost::array<uint8_t, 265> buf;
        
        Condition current_condition;
        std::function<void()> completer;
        bool current_condition_resolved = true;
};

#endif