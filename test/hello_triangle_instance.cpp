//
// Created by edimetia3d on 2019-04-23.
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
                                 "layout (location = 2) in vec2 offset;"
                                 "layout (location = 9) in vec3 aColor;"
                                 "out vec3 color;"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x + offset.x , aPos.y + offset.y, aPos.z, 1.0);\n"
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

    static void Describe() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointWithColor<T>),
                              (void *) offsetof(PointWithColor<T>, x));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(PointWithColor<T>),
                              (void *) offsetof(PointWithColor<T>, r));
        glEnableVertexAttribArray(9);
    }
};

struct Offset {
    float offset_x;
    float offset_y;

    static void Describe() {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Offset), (void *) offsetof(Offset, offset_x));
        glVertexAttribDivisor(2, 1);
    };
};

#define INSTANCE_NUM 5

struct SimpleTriangle {
    void Init() {

        auto &host_buf = vtx_array.GetVertexBuffer();
        host_buf.resize(3);
        memcpy(&host_buf[0].x, vertices, sizeof(vertices));
        vtx_array.SyncToDevice();

        auto &buf = instance_offset.GetHostBuffer();
        buf.resize(INSTANCE_NUM);
        for (int i = 1; i < INSTANCE_NUM; ++i) {
            buf[i].offset_x = -1.0f + i * 2.0 / INSTANCE_NUM;
        }

        instance_offset.SyncToDevice();

        vtx_array.AddInstanceBuffer(instance_offset);

        program.InitVtxFragByStr({vertexShaderSource}, {fragmentShaderSource});

    }

    void Display(ExGLFWwindow *window) {
        program.Active();
        vtx_array.DrawInstanceByIndex(GL_TRIANGLES, INSTANCE_NUM);
    }

    glShaderProgram_t program;
    glVertexArray_t<PointWithColor<float>> vtx_array;
    glArrayBuffer_t<Offset> instance_offset;
};

int main() {
    GLFWGuard glfw_guard;
    ExGLFWwindow window;
    window.Init("Hello Triangle Instance");
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