
add_subdirectory(ui)


add_library(glpp_core
    shader_program.cc
    buffer.cc
    vertex_array.cc
    texture.cc
    stb_stub.cc
)
target_link_libraries(glpp_core
    PUBLIC glm
    PRIVATE ui_window stb
)