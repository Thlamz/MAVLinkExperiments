#include "message_checkers.hpp"
#include <iostream>

bool check_empty(mavlink_message_t) {
    return true;
}

Condition get_check_cmd_ack(uint16_t command) {
    return [command](mavlink_message_t message) {
        if(message.msgid = MAVLINK_MSG_ID_COMMAND_ACK) {
            mavlink_command_ack_t ack;
            mavlink_msg_command_ack_decode(&message, &ack);
            if(ack.command == command) {
                return true;
            }
        }
        return false;
    };
}

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

bool check_arm(mavlink_message_t message) {
    if(message.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(&message, &heartbeat);
        return (heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED);
    }
    return false;
}

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