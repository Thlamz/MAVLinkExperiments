#include <ardupilotmega/mavlink.h>
#include "requirement.hpp"

bool check_pre_arm(mavlink_message_t message);

bool check_arm(mavlink_message_t message);