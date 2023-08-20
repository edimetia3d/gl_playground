//
// License: MIT
//

#ifndef GLPP_SRC_UI_INITIALIZER_H
#define GLPP_SRC_UI_INITIALIZER_H
namespace glpp::ui {
struct GLADInitializer {
  GLADInitializer() {
    if (gladLoadGL(glfwGetProcAddress) == 0) {
      std::cerr << "Cannot load opengl function" << std::endl;
      exit(-1);
    }
  }
};

struct GLFWInitializer {
  GLFWInitializer() {
    if (glfwInit() == 0) {
      std::cerr << "Cannot init glfw" << std::endl;
      exit(-1);
    }
  }
};
} // namespace glpp::ui
#endif // GLPP_SRC_UI_INITIALIZER_H
