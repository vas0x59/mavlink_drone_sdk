cmake_minimum_required (VERSION 2.6)
project (Mavilink_Indoor_SDK)

add_definitions("-std=gnu++11")
add_definitions("-D_USE_MATH_DEFINES")

include_directories("./src")
include_directories("./mavlink/v2.0/common")

add_library(mavlink_drone_sdk  SHARED
    ./src/udp.cpp
    ./src/tcp.cpp
    ./src/drone.cpp
    ./src/helpers.cpp
    ./src/serial_port.cpp
    ./src/autopilot_interface.cpp
    ./src/log.cpp
)

