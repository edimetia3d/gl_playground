cmake_minimum_required(VERSION 3.25)
project(glpp)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/configure_third_party.cmake)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

add_subdirectory(src)
add_subdirectory(test)
