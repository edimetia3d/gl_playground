//
// Created by edimetia3d on 2019/3/22.
//

#include <chrono>
#include <thread>

#include "include/ExGLFWwindow.h"

std::map<GLFWwindow *, ExGLFWwindow *> ExGLFWwindow::link_map;

void DefaultRenderFunction(ExGLFWwindow *the_window, double last_frame_duration) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

ExGLFWError ExGLFWwindow::Init(std::string window_name, int major_version, int minor_version) {
    if (inited_) {
        return ExGLFWError::GOOD;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major_version);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor_version);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int window_width = 800;
    int window_height = 600;
    window_ = glfwCreateWindow(window_width, window_height, window_name.c_str(), NULL, NULL);
    if (window_ == NULL) {
        glfwTerminate();
        return ExGLFWError::GLFW_INIT_ERROR;
    }
    link_map[window_] = this;

    // Load functions
    Active();
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        return ExGLFWError::GLAD_INIT_ERROR;
    }

    // Set resize callback
    // for retina display, buffer size is different than window size
    glfwGetFramebufferSize(window_, &buffer_width_, &buffer_height_);
    retina_ratio_ = buffer_width_ / window_width;
    glViewport(0, 0, buffer_width_, buffer_height_);
    glfwSetFramebufferSizeCallback(window_, FrameBufferResizeCallback);

    // Set mouse callback
    glfwSetCursorPosCallback(window_, ProcessMouseInput);

    // Set default render function
    render_function = DefaultRenderFunction;

    // Set Camera
    FrameBufferResizeCallback(window_, buffer_width_, buffer_height_);
    camera_.SetFOV(45);
    camera_.SetNearDist(0.1);
    camera_.SetFarDist(100);

    inited_ = true;
    return ExGLFWError::GOOD;

}

void ExGLFWwindow::Active() {
    glfwMakeContextCurrent(window_);
}

void ExGLFWwindow::FrameBufferResizeCallback(GLFWwindow *window, int buffer_width, int buffer_height) {
    glViewport(0, 0, buffer_width, buffer_height);
    auto ex_window = link_map[window];
    ex_window->buffer_width_ = buffer_width;
    ex_window->buffer_height_ = buffer_height;
    ex_window->camera_.SetWidth(buffer_width / ex_window->retina_ratio_);
    ex_window->camera_.SetHeight(buffer_height / ex_window->retina_ratio_);
}

int ExGLFWwindow::Show() {
    Active();
    while (!glfwWindowShouldClose(window_)) {
        last_frame_start_time_ = this_frame_start_time_;
        this_frame_start_time_ = glfwGetTime();
        render_function(this, this_frame_start_time_ - last_frame_start_time_);
        glfwSwapBuffers(window_);
        glfwPollEvents();
        ProcessKeyInput();
        double this_frame_duration = glfwGetTime() - this_frame_start_time_;
        std::this_thread::sleep_for(std::chrono::microseconds() * (333.33 - this_frame_duration));//FPS controll
    }
    return 0;
}

void ExGLFWwindow::SetRender(RenderFun_t fun) {
    render_function = fun;
}

void ExGLFWwindow::ProcessKeyInput() {
    camera_.UpdateByKey(window_, this_frame_start_time_ - last_frame_start_time_);
}

void ExGLFWwindow::ProcessMouseInput(GLFWwindow *window, double xpos, double ypos) {
    link_map[window]->camera_.UpdateByMouse(xpos, ypos);
}

