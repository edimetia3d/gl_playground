//
// Created by edimetia3d on 2019-03-29.
//

#include <memory>
#include <chrono>

#include <ExGLFWwindow.h>
#include <glShader_t.h>
#include <glVertexArray_t.h>
#include <glTexture2D_t.h>
#include <glTransform_t.h>
#include <glFrameBuffer_t.h>

#include "cube_point_data.h"

static std::string GetBuildDir() {
    std::string current_dir = __FILE__;
    int pos = current_dir.rfind('/');
    current_dir = std::string(current_dir.begin(), current_dir.begin() + pos);
    return current_dir;
}

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

struct CubeWithTexture {
    void Init() {
        auto &host_buf = vtx_array.GetVertexBuffer();
        host_buf.resize(36);
        memcpy(&host_buf[0].pos_x, vertices, sizeof(vertices));
        vtx_array.SyncToDevice();

        std::string build_dir = GetBuildDir();
        program.InitVtxFragByFile({build_dir + "/cube_with_texture.vert"}, {build_dir + "/cube_with_texture.frag"});

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

struct SquareWithOtherTexture {
    void Init(glTexture2D_t *texture) {
        the_texture = texture;
        auto &host_buf = vtx_array.GetVertexBuffer();
        host_buf.resize(6);
        float square_data[] = {
                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

                -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 0.0f, 1.0f, 1.0f
        };
        memcpy(&host_buf[0].pos_x, square_data, sizeof(square_data));
        vtx_array.SyncToDevice();

        std::string build_dir = GetBuildDir();
        program.InitVtxFragByFile({build_dir + "/texture_show.vert"}, {build_dir + "/texture_show.frag"});

        program.SetTexture(*the_texture, "screenTexture", 6);
    }

    void Display(ExGLFWwindow *window) {
        the_texture->Active();
        program.Active();
        vtx_array.DrawByIndex(GL_TRIANGLES);
    }

    glShaderProgram_t program;
    glVertexArray_t<PointWithTexturePos<float>> vtx_array;
    glTexture2D_t *the_texture;
};

int main() {
    GLFWGuard glfw_guard;
    ExGLFWwindow window;
    window.Init("3D World in FrameBuffer");
    window.EnableCursor(false);

    CubeWithTexture cube;
    cube.Init();

    glFrameBuffer_t<GL_FRAMEBUFFER> fb;
    fb.Init(window.GetBufferWidth(), window.GetBufferHeight(), ContainerFormat::TEXTURE_2D);

    SquareWithOtherTexture fb_show_squre;
    fb_show_squre.Init(fb.GetTexuture().get());

    double total_time = 0;


    auto render_fun = [&](ExGLFWwindow *in, double last_frame_duration) {

        // Draw into custom frame buffer
        fb.LazyResize(in->GetBufferWidth(), in->GetBufferHeight());
        fb.Active();
        total_time += last_frame_duration;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        auto look_trans = in->GetCameraTranform();

        for (int i = 0; i < 10; i += 2) {
            glTransform_t transform;
            transform.Rotate(-total_time * 32 * i, i, i, 0);
            transform.Translate(i, 0, 0);
            transform.Rotate(total_time * 8 * i, 0, i, i);

            cube.SetTransFrom(transform, "model_trans");
            cube.SetTransFrom(look_trans, "look_trans");

            cube.Display(in);
        }

        // Draw on screen

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        fb_show_squre.Display(in);
    };

    window.SetRender(render_fun);

    return window.Show();
}