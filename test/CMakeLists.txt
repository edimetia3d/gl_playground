
set(TEST_RESOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources")
configure_file(test_resource.h.in test_resource.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_LIST_DIR})

add_executable(hello_window 00_hello_window.cc)
target_link_libraries(hello_window PRIVATE ui_window)

add_executable(hello_triangle 01_hello_triangle.cc)
target_link_libraries(hello_triangle PRIVATE ui_window glpp_core)

add_executable(hello_camera 02_hello_camera.cc)
target_link_libraries(hello_camera PRIVATE ui_window glpp_core glpp_preset)

add_executable(hello_texture 03_hello_texture.cc)
target_link_libraries(hello_texture PRIVATE ui_window glpp_core)