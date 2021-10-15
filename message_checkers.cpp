#include "message_checkers.hpp"
#include <iostream>

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