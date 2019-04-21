//
// Created by edimetia3d on 2019-03-29.
//

#include <memory>

#include <ExGLFWwindow.h>
#include <glShader_t.h>
#include <glVertexArray_t.h>
#include <glTexture2D_t.h>
#include <glTransform_t.h>

static std::string GetBuildDir() {
    std::string current_dir = __FILE__;
    int pos = current_dir.rfind('/');
    current_dir = std::string(current_dir.begin(), current_dir.begin() + pos);
    return current_dir;
}

float vertices[] = {
        -0.25f, -0.25f, 0.0f, 0.0f, 0.0f,
        0.25f, -0.25f, 0.0f, 1.0f, 0.0f,
        0.25f, 0.25f, 0.0f, 1.0f, 1.0f,
        -0.25f, 0.25f, 0.0f, 0.0f, 1.0f,
};

template<class T>
struct PointWithTexturePos {
    T pos_x;
    T pos_y;
    T pos_z;
    T texture_x;
    T texture_y;
};

template<>
struct DefineVertexAttrib<PointWithTexturePos<float>> {
    static void Define() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointWithTexturePos<float>),
                              (void *) offsetof(PointWithTexturePos<float>, pos_x));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PointWithTexturePos<float>),
                              (void *) offsetof(PointWithTexturePos<float>, texture_x));
        glEnableVertexAttribArray(1);
    }
};

struct SquareWithTexture {
    void Init() {
        auto &host_buf = vtx_array.GetVertexBuffer();
        host_buf.resize(4);
        auto &host_index = vtx_array.GetIndexBuffer();
        host_index = {0, 1, 2, 0, 2, 3};
        memcpy(&host_buf[0].pos_x, vertices, sizeof(vertices));
        vtx_array.SyncToDevice();

        std::string build_dir = GetBuildDir();
        program.InitVtxFragByFile({build_dir + "/square_with_texture.vert"}, {build_dir + "/square_with_texture.frag"});

        tile_texture.LoadFromFile(build_dir + "/smile_face.png", GL_RGB, GL_RGBA);
        program.SetTexture(tile_texture, "tile_txture", 6);
    }

    void Display(ExGLFWwindow *window) {
        tile_texture.Active();
        program.Active();
        vtx_array.DrawByIndex(GL_TRIANGLES);
    }

    void SetTransFrom(glTransform_t &transform, const std::string &var_name_in_shader) {
        program.SetTransform(transform, var_name_in_shader);
    }

    glShaderProgram_t program;
    glVertexArray_t<PointWithTexturePos<float>> vtx_array;
    glTexture2D_t tile_texture;
};

int main() {
    GLFWGuard glfw_guard;
    ExGLFWwindow window;
    window.Init("Simple Transfrom");
    SquareWithTexture square;
    square.Init();

    double total_time = 0;
    auto render_fun = [&](ExGLFWwindow *in, double last_frame_duration) {

        glTransform_t transform;
        total_time += last_frame_duration;
        transform.Rotate(-total_time * 32, 0, 0, 1);
        transform.Translate(0.5, 0, 0);
        transform.Rotate(total_time * 8, 0, 0, 1);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        square.SetTransFrom(transform, "transform");
        square.Display(in);
    };
    window.SetRender(render_fun);

    return window.Show();
}