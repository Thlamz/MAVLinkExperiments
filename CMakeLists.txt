cmake_minimum_required(VERSION 3.0.0)
project(MAVLinkTest VERSION 0.1.0)

include_directories("./mavlink")
find_package(Boost 1.77 COMPONENTS coroutine REQUIRED)

add_executable(MAVTest main.cpp message_checkers.cpp requirement.cpp mavlink_helper.cpp command_routines.cpp telemetry_routines.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(MAVTest Boost::coroutine)