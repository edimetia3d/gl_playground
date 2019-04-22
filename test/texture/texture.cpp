//
// Created by edimetia3d on 2019-03-29.
//

#include <memory>

#include <ExGLFWwindow.h>
#include <glShader_t.h>
#include <glVertexArray_t.h>
#include <glTexture2D_t.h>

static std::string GetBuildDir() {
    std::string current_dir = __FILE__;
    int pos = current_dir.rfind('/');
    current_dir = std::string(current_dir.begin(), current_dir.begin() + pos);
    return current_dir;
}

float vertices[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.5f, 1.0f
};

template<class T>
struct PointWithTexturePos {
    T pos_x;
    T pos_y;
    T pos_z;
    T texture_x;
    T texture_y;

    static void Describe() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointWithTexturePos<T>),
                              (void *) offsetof(PointWithTexturePos<T>, pos_x));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PointWithTexturePos<T>),
                              (void *) offsetof(PointWithTexturePos<T>, texture_x));
        glEnableVertexAttribArray(1);
    }
};

struct TriangleWithTexture {
    void Init() {

        auto &host_buf = vtx_array.GetVertexBuffer();
        host_buf.resize(3);
        memcpy(&host_buf[0].pos_x, vertices, sizeof(vertices));
        vtx_array.SyncToDevice();
        std::string build_dir = GetBuildDir();
        program.InitVtxFragByFile({build_dir + "/triangle_with_texture.vert"},
                                  {build_dir + "/triangle_with_texture.frag"});

        tile_texture.LoadFromFile(build_dir + "/tile.jpg");
        program.SetTexture(tile_texture, "tile_txture", 6);
    }

    void Display(ExGLFWwindow *window) {
        tile_texture.Active();
        program.Active();
        vtx_array.DrawByIndex(GL_TRIANGLES);
    }

    glShaderProgram_t program;
    glVertexArray_t<PointWithTexturePos<float>> vtx_array;
    glTexture2D_t tile_texture;
};

int main() {
    GLFWGuard glfw_guard;
    ExGLFWwindow window;
    window.Init("Simple Texture");
    TriangleWithTexture triangle;
    triangle.Init();
    auto render_fun = [&triangle](ExGLFWwindow *in,double) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        triangle.Display(in);
    };
    window.SetRender(render_fun);

    return window.Show();
}