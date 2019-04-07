//
// Created by edimetia3d on 2019-03-29.
//

#ifndef GL_PLAYGROUND_GLTEXTURE2D_T_H
#define GL_PLAYGROUND_GLTEXTURE2D_T_H

#include <string>

#include <glad/glad.h>

#include "glShader_t.h"
#include "UnCopyable.h"

class glTexture2D_t : public UnCopyable {
public:
    glTexture2D_t() {
        glGenTextures(1, &handle_);
    }

    ~glTexture2D_t() {
        glDeleteTextures(1, &handle_);
    }

    void Active() {
        glActiveTexture(GL_TEXTURE0 + unit_id_);
        glBindTexture(GL_TEXTURE_2D, handle_);
    }

    void BindToShaderProgram(glShaderProgram_t &shader_program, std::string var_name_in_shader, int unit_id) {
        shader_program.Active();
        glUniform1i(glGetUniformLocation(shader_program.GetHandle(), var_name_in_shader.c_str()), unit_id);
        unit_id_ = unit_id;
        Active();
    }

    void LoadFromFile(const std::string &file_path,
                      GLenum device_type = GL_RGB,
                      GLenum host_type = GL_RGB,
                      GLenum host_data_type = GL_UNSIGNED_BYTE);

    void LoadFromBuffer(void *source,
                        int width,
                        int height,
                        GLenum device_type = GL_RGB,
                        GLenum host_type = GL_RGB,
                        GLenum host_data_type = GL_UNSIGNED_BYTE) {
        Active();
        glTexImage2D(GL_TEXTURE_2D, 0, device_type, width, height, 0, host_type, host_data_type, source);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    GLuint GetHandle() {
        return handle_;
    }

private:
    unsigned int handle_;
    int unit_id_ = 0;
};


#endif //GL_PLAYGROUND_GLTEXTURE2D_T_H
