//
// License: MIT
//

// window.cc will provide definitions for glad.

#include "glpp/ui/ui_window.h"

#include <iostream>
#include <map>

#include <cassert>

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>

#include <GLFW/glfw3.h>

#include "ui/initializer.h"

namespace glpp::ui {

class InstanceTracker {
public:
  static InstanceTracker &Instance() {
    static InstanceTracker instance;
    return instance;
  }
  void Reg(glpp::ui::ExGFLWWindow *p) {
    assert(p && p->Handle());
    info[p->Handle()] = p;
  }

  void Unreg(glpp::ui::ExGFLWWindow *p) {
    auto erased = info.erase(p->Handle());
    assert(erased == 1);
    if (info.empty()) {
      glfwTerminate();
    }
  }
  glpp::ui::ExGFLWWindow *Get(GLFWwindow *raw_p) {
    assert(raw_p && info.count(raw_p) == 1);
    return info[raw_p];
  }

private:
  InstanceTracker() = default;
  std::map<GLFWwindow *, glpp::ui::ExGFLWWindow *> info;
};

struct GLFWCallBack {
  static void CursorPosCB(GLFWwindow *window, double xpos, double ypos) {
    thread_local double last_x = xpos;
    thread_local double last_y = ypos;
    auto *p = InstanceTracker::Instance().Get(window);
    p->CurrentIOHandler()->OnMouseMove(xpos, ypos, xpos - last_x, ypos - last_y);
    last_x = xpos;
    last_y = ypos;
  }

  static void UpdateViewPort(GLFWwindow *window, int buffer_width, int buffer_height) {
    glViewport(0, 0, buffer_width, buffer_height);
  }

  static void KeyboardCB(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *p = InstanceTracker::Instance().Get(window);
    auto handler = p->CurrentIOHandler();
    auto on_press = handler->OnPressRegistry();
    auto on_release = handler->OnReleaseRegistry();
    auto on_repeate = handler->OnRepeatRegistry();
    if (action == GLFW_PRESS) {
      if (on_press.count(key) == 1) {
        on_press[key](scancode, mods);
      }
    } else if (action == GLFW_RELEASE) {
      if (on_release.count(key) == 1) {
        on_release[key](scancode, mods);
      }
    } else {
      assert(action == GLFW_REPEAT);
      if (on_repeate.count(key) == 1) {
        on_repeate[key](scancode, mods);
      }
    }
  }
  static void InstallIOCallBack(GLFWwindow *window) {
    glfwSetCursorPosCallback(window, CursorPosCB);
    glfwSetKeyCallback(window, KeyboardCB);
  }
  static void UninstallIOCallBack(GLFWwindow *window) {
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetKeyCallback(window, nullptr);
  }
};

glpp::ui::ExGFLWWindow::ExGFLWWindow(const std::string &window_name, int major_version,
                                     int minor_version) {
  static GLFWInitializer glfw_initializer;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  int window_width = 800;
  int window_height = 600;
  raw_window_ = glfwCreateWindow(window_width, window_height, window_name.c_str(), NULL, NULL);
  if (!raw_window_) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    exit(-1);
  }

  // Load functions
  Active();
  // init glad when first window is created
  static GLADInitializer glad_initializer;

  glfwSetFramebufferSizeCallback(raw_window_, GLFWCallBack::UpdateViewPort);
  InstanceTracker::Instance().Reg(this);
}
void ExGFLWWindow::Active() { glfwMakeContextCurrent(raw_window_); }

ExGFLWWindow::~ExGFLWWindow() {
  if (raw_window_) {
    glfwDestroyWindow(raw_window_);
  }
  InstanceTracker::Instance().Unreg(this);
}

void ExGFLWWindow::Show(ExGFLWWindow::RenderFnType fun, IOHandler *io_handler) {
  Active();
  if (io_handler) {
    io_handler->BindWindow(this);
    current_io_handler_ = io_handler;
    auto on_press = io_handler->OnPressRegistry();
    // inject a default handler for escape key
    if (on_press.count(GLFW_KEY_ESCAPE) == 0) {
      on_press[GLFW_KEY_ESCAPE] = [](int scancode, int modifier) {
        glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
      };
    }
    GLFWCallBack::InstallIOCallBack(raw_window_);
  } else {
    current_io_handler_ = nullptr;
    GLFWCallBack::UninstallIOCallBack(raw_window_);
  }
  RenderFunArg arg{this};

  while (!glfwWindowShouldClose(raw_window_)) {
    fun(&arg);
    glfwSwapBuffers(raw_window_);
    glfwPollEvents();
  }
}
} // namespace glpp::ui