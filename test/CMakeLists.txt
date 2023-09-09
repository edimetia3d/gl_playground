
add_executable(hello_window 00_hello_window.cc)
target_link_libraries(hello_window PRIVATE ui_window)

add_executable(hello_triangle 01_hello_triangle.cc)
target_link_libraries(hello_triangle PRIVATE ui_window glpp_core)

add_executable(hello_camera 02_hello_camera.cc)
target_link_libraries(hello_camera PRIVATE ui_window glpp_core glpp_preset)