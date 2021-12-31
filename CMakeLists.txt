cmake_minimum_required(VERSION 3.10)
project( DisplayMotion )


# Add the path to the header file:
include_directories(./include/Cam.hpp)

# GLOB allows for wildcard additions to include your code:
file(GLOB SOURCES "./include/Cam.cpp" "./main.cpp")

# Add the openCV parts:
find_package( OpenCV REQUIRED )

# Combine everything and give the executable the name 'displayMotion'
add_executable( displayMotion ${SOURCES} )
target_link_libraries( displayMotion ${OpenCV_LIBS} )
