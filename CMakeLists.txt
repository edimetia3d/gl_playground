cmake_minimum_required(VERSION 3.13)
project(gl_playground)

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-m64)

find_package(OpenGL REQUIRED)

#dependence

include_directories(dependences/glfw/include)

include_directories(dependences/glad/include)
file(GLOB GLAD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/dependences/glad/src/*.c*)

include_directories(dependences/glm)

include_directories(dependences/assimp/include)

#project
file(GLOB gl_playground_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c*
                            ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*)

add_library(gl_playground STATIC ${gl_playground_SRC} ${GLAD_SRC})

add_subdirectory(test)