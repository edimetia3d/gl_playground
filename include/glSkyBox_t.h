//
// Created by edimetia3d on 2019-04-16.
//

#ifndef GL_PLAYGROUND_GLSKYBOX_T_H
#define GL_PLAYGROUND_GLSKYBOX_T_H

#include <string>
#include <glad/glad.h>

#include "glTextureCubeMap_t.h"
#include "glVertexArray_t.h"
#include "glShader_t.h"
#include "UnCopyable.h"
#include "glTransform_t.h"

template<class T>
struct SkyBoxPoint {
    T pos_x;
    T pos_y;
    T pos_z;
};

template<>
struct DefineVertexAttrib<SkyBoxPoint<float>> {
    static void Define() {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkyBoxPoint<float>),
                              (void *) offsetof(SkyBoxPoint<float>, pos_x));
        glEnableVertexAttribArray(0);
    }
};

class glSkyBox_t : public UnCopyable {
public:
    void Init(std::string base_dir, std::string file_type = ".jpg");
    void
    Init(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back);

    void Display(glTransform_t view_only, glTransform_t projection_only);

private:
    glTextureCubeMap_t cube_texture;
    glShaderProgram_t shader;
    glVertexArray_t<SkyBoxPoint<float>> cube_vertex;
};

#endif //GL_PLAYGROUND_GLSKYBOX_T_H
