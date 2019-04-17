//
// Created by edimetia3d on 2019-03-29.
//

#include <memory>
#include <chrono>

#include <ExGLFWwindow.h>

#include <glSkyBox_t.h>

static std::string GetBuildDir() {
    std::string current_dir = __FILE__;
    int pos = current_dir.rfind('/');
    current_dir = std::string(current_dir.begin(), current_dir.begin() + pos);
    return current_dir;
}


int main() {
    GLFWGuard glfw_guard;
    ExGLFWwindow window;
    window.Init("Just SkyBox");
    window.EnableCursor(false);

    glSkyBox_t skybox;
    skybox.Init(GetBuildDir() + "/skybox");

    glEnable(GL_DEPTH_TEST);
    auto render_fun = [&](ExGLFWwindow *in, double last_frame_duration) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        skybox.Display(in->GetCameraViewOnly(), in->GetCameraProjectionOnly());
    };

    window.SetRender(render_fun);

    return window.Show();
}