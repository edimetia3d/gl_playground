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

#include "cube_point_data.h"

static std::string GetBuildDir() {
    std::string current_dir = __FILE__;
    int pos = current_dir.rfind('/');
    current_dir = std::string(current_dir.begin(), current_dir.begin() + pos);
    return current_dir;
}

template<class T>
struct Point_Normal_TexturePos {
    T pos_x;
    T pos_y;
    T pos_z;
    T norm_x;
    T norm_y;
    T norm_z;
    T texture_x;
    T texture_y;
};

template<>
struct DefineVertexAttrib<Point_Normal_TexturePos<float>> {
    static void Define() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Point_Normal_TexturePos<float>),
                              (void *) offsetof(Point_Normal_TexturePos<float>, pos_x));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Point_Normal_TexturePos<float>),
                              (void *) offsetof(Point_Normal_TexturePos<float>, norm_x));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Point_Normal_TexturePos<float>),
                              (void *) offsetof(Point_Normal_TexturePos<float>, texture_x));
        glEnableVertexAttribArray(2);
    }
};

struct Container {
    void Init() {
        auto &host_buf = vtx_array.GetVertexBuffer();
        host_buf.resize(36);
        memcpy(&host_buf[0].pos_x, vertices, sizeof(vertices));
        vtx_array.SyncToDevice();

        std::string build_dir = GetBuildDir();
        program.InitVtxFragByFile({build_dir + "/container.vert"}, {build_dir + "/container.frag"});

        diffuse_texture.LoadFromFile(build_dir + "/container.png", GL_RGB, GL_RGBA);
        program.SetTexture(diffuse_texture, "material.diffuse", 0);

        specular_texture.LoadFromFile(build_dir+"/container_specular.png",GL_RGB,GL_RGBA);
        program.SetTexture(specular_texture, "material.specular", 1);
    }

    void SetLightColor(float r,float g,float b) {
        program.Active();
        glUniform3f(glGetUniformLocation(program.GetHandle(), "light.ambient"), 0.2f*r,0.2f*g,0.2f*b);
        glUniform3f(glGetUniformLocation(program.GetHandle(), "light.diffuse"), 0.5f*r,0.5f*g,0.5f*b);
        glUniform3f(glGetUniformLocation(program.GetHandle(), "light.specular"),1.0f*r,1.0f*g,1.0f*b);
    }

    void SetShiniess(float v){
        program.Active();
        glUniform1f(glGetUniformLocation(program.GetHandle(), "material.shininess"),v);
    }
    void SetLightPos(float x,float y,float z){
        program.Active();
        glUniform3f(glGetUniformLocation(program.GetHandle(),"light.position"),x,y,z);
    }

    void SetCameraPos(float x,float y,float z){
        program.Active();
        glUniform3f(glGetUniformLocation(program.GetHandle(),"viewPos"),x,y,z);
    }

    void Display(ExGLFWwindow *window) {
        diffuse_texture.Active();
        specular_texture.Active();
        program.Active();
        vtx_array.DrawByIndex(GL_TRIANGLES);
    }

    void SetTransFrom(glTransform_t &transform, const std::string &var_name_in_shader) {
        program.SetTransform(transform, var_name_in_shader);
    }

    glShaderProgram_t program;
    glVertexArray_t<Point_Normal_TexturePos<float>> vtx_array;
    glTexture2D_t diffuse_texture;
    glTexture2D_t specular_texture;
};

/**
 * LightCube is just a cube that looks white
 */
struct LightCube {
    void Init() {
        auto &host_buf = vtx_array.GetVertexBuffer();
        host_buf.resize(36);
        memcpy(&host_buf[0].pos_x, vertices, sizeof(vertices));
        vtx_array.SyncToDevice();
        std::string build_dir = GetBuildDir();
        program.InitVtxFragByFile({build_dir + "/light.vert"}, {build_dir + "/light.frag"});
    }

    void SetColor(float r,float g,float b) {
        program.Active();
        glUniform3f(glGetUniformLocation(program.GetHandle(), "light_color"), r,g,b);
    }
    void Display(ExGLFWwindow *window) {
        program.Active();
        vtx_array.DrawByIndex(GL_TRIANGLES);
    }

    void SetTransFrom(glTransform_t &transform, const std::string &var_name_in_shader) {
        program.SetTransform(transform, var_name_in_shader);
    }
    glShaderProgram_t program;
    glVertexArray_t<Point_Normal_TexturePos<float>> vtx_array;
};

int main() {
    GLFWGuard glfw_guard;
    ExGLFWwindow window;
    window.Init("3D World");
    window.EnableCursor(false);
    Container box;
    box.Init();

    LightCube the_light;// it is always in the center of the world
    the_light.Init();
    the_light.SetColor(1,1,1);
    box.SetLightColor(1,1,1);
    box.SetShiniess(64);

    double total_time = 1000;
    glEnable(GL_DEPTH_TEST);// Enable Depth Test globaly

    auto render_fun = [&](ExGLFWwindow *in, double last_frame_duration) {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto look_trans = in->GetCameraTranform();
        auto camera_pos=in->GetCameraPos();
        box.SetCameraPos(camera_pos.x,camera_pos.y,camera_pos.z);
        // Set light pos
        {
            glTransform_t transform;
            transform.Scale(0.1,0.1,0.1);
            transform.Translate(1,1,1);
            the_light.SetTransFrom(transform,"model_trans");
            box.SetLightPos(1,1,1);

            the_light.SetTransFrom(look_trans,"look_trans");
        }
        for (int i = 0; i < 10; i += 2) {
            glTransform_t transform;
            transform.Rotate(-total_time * 32 * i, i, i, 0);
            transform.Translate(i, 0, 0);
            transform.Rotate(total_time * 8 * i, 0, i, i);

            box.SetTransFrom(transform, "model_trans");
            box.SetTransFrom(look_trans, "look_trans");

            box.Display(in);
        }
        the_light.Display(in);
    };

    window.SetRender(render_fun);

    return window.Show();
}