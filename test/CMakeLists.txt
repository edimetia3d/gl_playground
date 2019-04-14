link_libraries(gl_playground)

option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
link_directories(${CMAKE_BINARY_DIR}/dependences/glfw/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/dependences/glfw ${CMAKE_BINARY_DIR}/dependences/glfw)

set(old_shared_libs_para ${BUILD_SHARED_LIBS})
OPTION( BUILD_SHARED_LIBS "Build package with shared libraries." OFF)
OPTION ( ASSIMP_BUILD_TESTS "If the test suite for Assimp is built in addition to the library." OFF)
add_subdirectory(${CMAKE_SOURCE_DIR}/dependences/assimp ${CMAKE_BINARY_DIR}/dependences/assimp)
OPTION(BUILD_SHARED_LIBS "Build package with shared libraries." ${old_shared_libs_para})

link_libraries(${OPENGL_gl_LIBRARY} glfw3 assimp)

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    link_libraries(gl_playground ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif (APPLE)

include_directories(../include)

add_executable(simple_window simple_window.cpp)

add_executable(hello_triangle hello_triangle.cpp)

add_executable(texture ./texture/texture.cpp)

add_executable(transfrom_2d ./transfrom_2d/transfrom2d.cpp)

add_executable(transform_3d ./transform_3d/transform3d.cpp)

add_executable(simple_light ./simple_light/simple_light.cpp)

add_executable(simple_assimp ./simple_assimp/simple_assimp.cpp)

add_executable(simple_frame_buffer ./simple_frame_buffer/simple_frame_buffer.cpp)