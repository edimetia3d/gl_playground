cmake_minimum_required(VERSION 3.25)
project(glpp)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/configure_third_party.cmake)

add_executable(glpp main.cpp)
