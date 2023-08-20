//
// License: MIT
//
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <GLFW/glfw3.h>

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  GLFWwindow *window = glfwCreateWindow(800, 600, "Hello, World!", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  gladLoadGL(glfwGetProcAddress);
  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}