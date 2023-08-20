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

CPMAddPackage(
    NAME GH_GLAD
    GITHUB_REPOSITORY "Dav1dde/glad"
    GIT_TAG "2348b07c1ab4504d60398713781d8a57880234fa"
    EXCLUDE_FROM_ALL TRUE
    CPM_USE_NAMED_CACHE_DIRECTORIES TRUE
    DOWNLOAD_ONLY TRUE
)

function(__generate_glad)
  set(GEN_GALD_OUTPUT_DIR ${GH_GLAD_BINARY_DIR}/generated/${GLPP_CORE_PROFILE_VERSION})

  set(GLAD_H_FILE ${GEN_GALD_OUTPUT_DIR}/include/glad/gl.h)
  # check if glad.h exists
  if (EXISTS ${GLAD_H_FILE})
    return()
  endif ()

  find_package(Python3 COMPONENTS Interpreter REQUIRED)
  execute_process(
      COMMAND bash -c "${Python3_EXECUTABLE} -m venv ${GH_GLAD_BINARY_DIR}/venv"
      WORKING_DIRECTORY ${GH_GLAD_SOURCE_DIR}
      RESULT_VARIABLE ret
  )
  if (NOT ret EQUAL 0)
    message(FATAL_ERROR "Failed to generate glad with code ${ret}")
  endif ()
  execute_process(
      COMMAND bash -c "${GH_GLAD_BINARY_DIR}/venv/bin/pip3 install -r ${GH_GLAD_SOURCE_DIR}/requirements.txt"
      WORKING_DIRECTORY ${GH_GLAD_SOURCE_DIR}
      RESULT_VARIABLE ret
  )
  if (NOT ret EQUAL 0)
    message(FATAL_ERROR "Failed to generate glad with code ${ret}")
  endif ()
  execute_process(
      COMMAND bash -c "${GH_GLAD_BINARY_DIR}/venv/bin/python3 -m glad --out-path=${GEN_GALD_OUTPUT_DIR} --api=gl:core=${GLPP_CORE_PROFILE_VERSION} c --header-only"
      WORKING_DIRECTORY ${GH_GLAD_SOURCE_DIR}
      RESULT_VARIABLE ret
  )
  # check ret code
  if (NOT ret EQUAL 0)
    message(FATAL_ERROR "Failed to generate glad with code ${ret}")
  endif ()
endfunction()

#check GLPP_CORE_PROFILE_VERSION defined
if (NOT DEFINED GLPP_CORE_PROFILE_VERSION)
  set(GLPP_CORE_PROFILE_VERSION 4.2)
  message(WARNING "GLPP_CORE_PROFILE_VERSION not defined, use ${GLPP_CORE_PROFILE_VERSION} as default")
endif ()
__generate_glad()
add_library(glad INTERFACE)
target_include_directories(glad INTERFACE ${GH_GLAD_BINARY_DIR}/generated/${GLPP_CORE_PROFILE_VERSION}/include)