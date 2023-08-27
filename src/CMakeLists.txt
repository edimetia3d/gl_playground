
add_subdirectory(ui)


add_library(glpp_core
    shader_program.cc
    buffer.cc
    vertex_array.cc
)
target_link_libraries(glpp_core PRIVATE ui_window)