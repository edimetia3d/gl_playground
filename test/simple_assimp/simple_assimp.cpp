//
// Created by edimetia3d on 2019-03-29.
//

#include <memory>

#include <ExGLFWwindow.h>
#include <glShader_t.h>
#include <glVertexArray_t.h>
#include <glTexture2D_t.h>
#include <AssimpModel_t.h>

static std::string GetBuildDir() {
    std::string current_dir = __FILE__;
    int pos = current_dir.rfind('/');
    current_dir = std::string(current_dir.begin(), current_dir.begin() + pos);
    return current_dir;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("ERROR: Must input a model file \n");
        return -1;
    }
    GLFWGuard glfw_guard;
    ExGLFWwindow window;
    window.Init("Simple Assimp");
    window.EnableCursor(false);

    AssimpModel assimp_model;
    assimp_model.loadModel(argv[1]);

    glShaderProgram_t shader;
    shader.InitByFile({GetBuildDir() + "/simple_assimp.vert"}, {GetBuildDir() + "/simple_assimp.frag"});


    glEnable(GL_DEPTH_TEST);// Enable Depth Test globaly
    auto render_fun = [&assimp_model, &shader](ExGLFWwindow *in, double) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        auto view = in->GetCameraTranform();
        shader.SetTransform(view, "view");
        glTransform_t model_trans;
        shader.SetTransform(model_trans, "model");
        assimp_model.Draw(shader);
    };

    window.SetRender(render_fun);

    return window.Show();
}