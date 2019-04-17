//
// Created by edimetia3d on 2019-04-17.
//

#include "include/glSkyBox_t.h"

static char vert_code[] = "#version 330 core\n"
                          "layout (location = 0) in vec3 aPos;\n"
                          "\n"
                          "out vec3 TexCoords;\n"
                          "\n"
                          "uniform mat4 view;\n"
                          "\n"
                          "void main()\n"
                          "{\n"
                          "    TexCoords = aPos;\n"
                          "    vec4 pos = view * vec4(aPos, 1.0);\n"
                          "    gl_Position = pos.xyww;\n"
                          "}";
static char frag_code[] = "#version 330 core\n"
                          ""
                          "out vec4 FragColor;\n"
                          "in vec3 TexCoords;\n"
                          "uniform samplerCube skybox;\n"
                          "void main()\n"
                          "{    \n"
                          "    FragColor = texture(skybox, TexCoords);\n"
                          "}";
static float skyboxVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
};

void glSkyBox_t::Init(std::string base_dir) {
    Init(base_dir + "/right.jpg",
         base_dir + "/left.jpg",
         base_dir + "/top.jpg",
         base_dir + "/bottom.jpg",
         base_dir + "/front.jpg",
         base_dir + "/back.jpg");
}

void glSkyBox_t::Init(std::string right, std::string left, std::string top, std::string bottom, std::string front,
                      std::string back) {
    cube_texture.LoadFromFile({right, left, top, bottom, front, back});
    shader.InitByStr({std::string(vert_code)}, {std::string(frag_code)});
    auto &vertex = cube_vertex.GetVertexBuffer();
    shader.SetTexture(cube_texture, "skybox", 0);
    vertex.resize(36);
    memcpy(&vertex[0], skyboxVertices, sizeof(skyboxVertices));
    cube_vertex.SyncToDevice();
}

void glSkyBox_t::Display(glTransform_t view_only, glTransform_t projection_only) {
    float (*f)[4] = (float (*)[4]) view_only.Data();
    f[3][0] /= 300;
    f[3][1] /= 300;
    f[3][2] /= 300;
    projection_only.RightMul(view_only);
    shader.SetTransform(projection_only, "view");
    shader.Active();
    cube_texture.Active();
    GLint bk;
    glGetIntegerv(GL_DEPTH_FUNC, &bk);
    glDepthFunc(GL_LEQUAL);
    cube_vertex.DrawByIndex(GL_TRIANGLES);
    glDepthFunc(bk);
}


