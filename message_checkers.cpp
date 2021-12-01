#include "message_checkers.hpp"
#include <iostream>
/**
 * @brief This module contains message checkers that should be used as conditions for requirements.
 * 
 */



/**
 * @brief An empty checker, always immediatly resolves
 * 
 * @return true 
 */
bool check_empty(mavlink_message_t) {
    return true;
}


/**
 * @brief Gets the check_cmd_ack message checker for a specific command
 * 
 * @param command The command you want to get the checker for
 * @return Condition Returns true when command gets acknowledged
 */
Condition get_check_cmd_ack(uint16_t command) {
    return [command](mavlink_message_t message) {
        if(message.msgid = MAVLINK_MSG_ID_COMMAND_ACK) {
            mavlink_command_ack_t ack;
            mavlink_msg_command_ack_decode(&message, &ack);
            if(ack.command == command && ack.result == MAV_RESULT_ACCEPTED) {
                return true;
            }
        }
        return false;
    };
}

/**
 * @brief Checks if the vehicle is ready to arm
 * 
 * @param message Telemetry recieved
 * @return true If the vehicle is ready to arm
 * @return false If the vehicle is not yet ready to arm 
 */
bool check_pre_arm(mavlink_message_t message) {
    uint16_t required_value = (EKF_ATTITUDE |
                ESTIMATOR_VELOCITY_HORIZ |
                ESTIMATOR_VELOCITY_VERT |
                ESTIMATOR_POS_HORIZ_REL |
                ESTIMATOR_PRED_POS_HORIZ_REL);
                    
    uint16_t error_bits = (ESTIMATOR_CONST_POS_MODE |
                ESTIMATOR_ACCEL_ERROR);
    if(message.msgid == MAVLINK_MSG_ID_EKF_STATUS_REPORT) {
        mavlink_ekf_status_report_t report;
        mavlink_msg_ekf_status_report_decode(&message, &report);
        
        // Flags don't include error bits and include all required values
        return !(report.flags & error_bits) && ((report.flags & required_value) == required_value);
    }

    return false;
}

/**
 * @brief Checks if the vehicle is armed
 * 
 * @param message Telemetry received
 * @return true If the vehicle is armed
 * @return false If the vehicle is not armed
 */
bool check_arm(mavlink_message_t message) {
    if(message.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(&message, &heartbeat);
        return (heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED);
    }
    return false;
}

/**
 * @brief Gets a message checker that will verify if a vehicle is within tolerance of an altutude
 * 
 * @param altitude Target altitude
 * @param tolerance Margin of error for the altitude
 * @return Condition Message checker for that specific altitude and tolerance
 */
Condition get_check_altitude(int32_t altitude, int32_t tolerance) {
    return [=](mavlink_message_t msg) {
        if(msg.msgid == MAVLINK_MSG_ID_GLOBAL_POSITION_INT) {
            mavlink_global_position_int_t position;
            mavlink_msg_global_position_int_decode(&msg, &position);
            return abs(position.relative_alt / 1000 - altitude) < tolerance;
        }
        return false;
    };
}