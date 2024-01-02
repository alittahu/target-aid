cmake_minimum_required(VERSION 3.16)
project(target_aid)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE on)
set(JSON_SOURCE "${CMAKE_SOURCE_DIR}/resources/guns.json")
set(JSON_DESTINATION "${CMAKE_BINARY_DIR}/guns.json")

# Fallback for windows if opencv is installed with choco
if ((NOT DEFINED OpenCV_DIR OR OpenCV_DIR STREQUAL "OpenCV_DIR-NOTFOUND") AND ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(OpenCV_DIR "C:/tools/opencv/build")
endif()

message(STATUS "OpenCV_DIR set to: ${OpenCV_DIR}")
find_package(OpenCV REQUIRED)

include_directories(
        headers
        ${OpenCV_INCLUDE_DIRS}
)

link_libraries(
        ${OpenCV_LIBS}
)

add_executable(target_aid
        cpp/main.cpp
        cpp/TargetDetector.cpp
        headers/TargetDetector.h
        headers/Gun.h
        headers/json.hpp
        cpp/Gun.cpp
        cpp/ShootingRange.cpp
        headers/ShootingRange.h
        cpp/TargetTracker.cpp
        headers/TargetTracker.h
        cpp/Bullet.cpp
        headers/Bullet.h)

add_custom_command(
        TARGET target_aid POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${JSON_SOURCE} ${JSON_DESTINATION}
)
