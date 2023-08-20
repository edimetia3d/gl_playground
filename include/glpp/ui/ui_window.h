//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_UI_UI_WINDOW_H
#define GLPP_INCLUDE_GLPP_UI_UI_WINDOW_H

#include <functional>
#include <string>

#include <glpp/ui/io_handler.h>

#include <glad/gl.h>
// glad must be included before glfw3.h
#include <GLFW/glfw3.h>

namespace glpp::ui {

enum class ExGFLWWindowError {
  NO_ERROR,
  GLFW_WINDOW_CREATION_FAIL,
};

/**
 * A Wrapper class to help create a simple GLFW window. It basically does these things:
 * 1. Init OPENGL and underlying GFLW.
 * 2. Provide hooks for user to handle IO.
 * 3. Manage the core render loop.
 */
class ExGFLWWindow {
public:
  struct RenderFunArg {
    ExGFLWWindow *window;
  };

  using RenderFnType = std::function<void(RenderFunArg *arg)>;

  ExGFLWWindow(const std::string &window_name, int major_version = 3, int minor_version = 3);

  ~ExGFLWWindow();

  void Show(ExGFLWWindow::RenderFnType fun, IOHandler *io_handler);

  void Active();

  GLFWwindow *Handle() { return raw_window_; }

  IOHandler *CurrentIOHandler() { return current_io_handler_; }

private:
  GLFWwindow *raw_window_ = nullptr;
  IOHandler *current_io_handler_ = nullptr;
};
} // namespace glpp::ui

#endif // GLPP_INCLUDE_GLPP_UI_UI_WINDOW_H
