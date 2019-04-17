//
// Created by edimetia3d on 2019/3/22.
//

#ifndef OPENGL_EXGLFWWINDOW_H
#define OPENGL_EXGLFWWINDOW_H

#include <string>
#include <map>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GLFW_FPS_Camera.h"

enum class ExGLFWError {
    GOOD,
    GLFW_INIT_ERROR,
    GLAD_INIT_ERROR
};

class GLFWGuard {
public:
    GLFWGuard() {
        glfwInit();
    }

    ~GLFWGuard() {
        glfwTerminate();
    }
};

class ExGLFWwindow {
public:
    /**
     * Render may need infomations from window
     */
    using RenderFun_t=std::function<void(ExGLFWwindow *, double)>;

    ~ExGLFWwindow() {
        if (window_) {
            glfwDestroyWindow(window_);
            link_map.erase(window_);
        }
    }

    /**
     * Init is the ctor, so one instance could only be init one time, after success init ,all call will return directly
     */
    ExGLFWError Init(std::string window_name, int major_version = 3, int minor_version = 3);

    /**
     * Make the window as current opengl context
     */
    void Active();

    void SetRender(RenderFun_t fun);

    glTransform_t GetCameraTranform() {
        return camera_.GetTransfrom();
    }

    glTransform_t GetCameraViewOnly() {
        return camera_.GetViewOnly();
    }

    glTransform_t GetCameraProjectionOnly() {
        return camera_.GetProjectionOnly();
    }

    glm::vec3 GetCameraPos(){
        return camera_.GetCameraPos();
    }

    void EnableCursor(bool enable = true) {
        if (enable) {
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }

    /**
     * show the window, this will call Active() automatically
     */
    int Show();

    int GetBufferWidth() {
        return buffer_width_;
    }

    int GetBufferHeight() {
        return buffer_height_;
    }
private:
    GLFWwindow *window_ = nullptr;
    int buffer_width_;
    int buffer_height_;

    float retina_ratio_=1;
    RenderFun_t render_function;
    bool inited_ = false;
    GLFW_FPS_Camera camera_;

    double last_frame_start_time_ = 0;
    double this_frame_start_time_ = 0;

    static std::map<GLFWwindow *, ExGLFWwindow *> link_map;

    static void FrameBufferResizeCallback(GLFWwindow *window, int buffer_width, int buffer_height);

    void ProcessKeyInput();

    static void ProcessMouseInput(GLFWwindow *window, double xpos, double ypos);
};


#endif //OPENGL_EXGLFWWINDOW_H
