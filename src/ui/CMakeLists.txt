
add_library(ui_window ui_window.cc io_handler.cc)
target_link_libraries(ui_window PUBLIC glad glfw)