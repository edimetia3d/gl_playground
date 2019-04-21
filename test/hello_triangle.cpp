//
// Created by edimetia3d on 2019-03-26.
//

#include <ExGLFWwindow.h>
#include <glShader_t.h>
#include <glVertexArray_t.h>

float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "layout (location = 9) in vec3 aColor;"
                                 "out vec3 color;"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "   color=aColor;"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 color;"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(color, 1.0f);\n"
                                   "}\n\0";

template<class T>
struct PointWithColor {
    T x;
    T y;
    T z;
    T r;
    T g;
    T b;
};

template<>
struct DefineVertexAttrib<PointWithColor<float>> {
    static void Define() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointWithColor<float>),
                              (void *) offsetof(PointWithColor<float>, x));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(PointWithColor<float>),
                              (void *) offsetof(PointWithColor<float>, r));
        glEnableVertexAttribArray(9);
    }
};

struct SimpleTriangle {
    void Init() {

        auto &host_buf = vtx_array.GetVertexBuffer();
        host_buf.resize(3);
        memcpy(&host_buf[0].x, vertices, sizeof(vertices));
        vtx_array.SyncToDevice();

        program.InitVtxFragByStr({vertexShaderSource}, {fragmentShaderSource});

    }

    void Display(ExGLFWwindow *window) {
        program.Active();
        vtx_array.DrawByIndex(GL_TRIANGLES);
    }

    glShaderProgram_t program;
    glVertexArray_t<PointWithColor<float>> vtx_array;
};

int main() {
    GLFWGuard glfw_guard;
    ExGLFWwindow window;
    window.Init("Hello Triangle");
    SimpleTriangle triangle;
    triangle.Init();
    auto render_fun = [&triangle](ExGLFWwindow *in, double last_frame_duration) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        triangle.Display(in);
    };
    window.SetRender(render_fun);

    return window.Show();
}