
add_library(glpp_preset fps_io.cc fps_camera.cc)
target_link_libraries(glpp_preset PUBLIC ui_window glm)