cmake_minimum_required(VERSION 4.0)
project(channel)

set(CMAKE_CXX_STANDARD 20)

add_library(buffered_channel INTERFACE)
target_sources(buffered_channel INTERFACE
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        buffered_channel.h
)
add_executable(channel main.cpp)
