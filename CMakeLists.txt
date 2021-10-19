cmake_minimum_required(VERSION 3.0.0)
project(MAVTest VERSION 0.1.0)

include(CTest)
include_directories("./mavlink")
include_directories("H:\\Installs\\Ferramentas\\boost_1_77_0")
link_directories("H:\\Installs\\Ferramentas\\boost_1_77_0\\stage\\lib")

enable_testing()

add_executable(MAVTest main.cpp message_checkers.cpp mavlink_helper.cpp command_routines.cpp telemetry_routines.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
