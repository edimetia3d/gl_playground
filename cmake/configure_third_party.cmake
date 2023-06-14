include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

CPMAddPackage(
    NAME GH_GLM
    GITHUB_REPOSITORY "g-truc/glm"
    GIT_TAG "efec5db081e3aad807d0731e172ac597f6a39447"
    EXCLUDE_FROM_ALL TRUE
    CPM_USE_NAMED_CACHE_DIRECTORIES TRUE
    DOWNLOAD_ONLY TRUE
)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${GH_GLM_SOURCE_DIR})

CPMAddPackage(
    NAME GH_ASSIMP
    GITHUB_REPOSITORY "assimp/assimp"
    GIT_TAG "9519a62dd20799c5493c638d1ef5a6f484e5faf1"
    OPTIONS "ASSIMP_BUILD_TESTS OFF" "ASSIMP_INSTALL OFF" "ASSIMP_BUILD_ASSIMP_VIEW OFF"
    EXCLUDE_FROM_ALL TRUE
    CPM_USE_NAMED_CACHE_DIRECTORIES TRUE
)

CPMAddPackage(
    NAME GH_GLFW
    GITHUB_REPOSITORY "glfw/glfw"
    GIT_TAG "7482de6071d21db77a7236155da44c172a7f6c9e"
    OPTIONS "GLFW_BUILD_TESTS OFF" "GLFW_BUILD_EXAMPLES OFF" "GLFW_BUILD_DOCS OFF" "GLFW_INSTALL OFF"
    EXCLUDE_FROM_ALL TRUE
    CPM_USE_NAMED_CACHE_DIRECTORIES TRUE
)