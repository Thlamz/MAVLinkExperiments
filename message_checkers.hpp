#include <ardupilotmega/mavlink.h>
#include "requirement.hpp"

bool check_empty(mavlink_message_t);

Condition get_check_cmd_ack(uint16_t command);

bool check_pre_arm(mavlink_message_t message);

bool check_arm(mavlink_message_t message);

Condition get_check_altitude(int32_t altitude, int32_t tolerance);