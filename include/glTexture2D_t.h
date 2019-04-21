//
// Created by edimetia3d on 2019-03-29.
//

#ifndef GL_PLAYGROUND_GLTEXTURE2D_T_H
#define GL_PLAYGROUND_GLTEXTURE2D_T_H

#include <string>

#include <glad/glad.h>

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
        if (unit_id_ != -1) {
            glActiveTexture(GL_TEXTURE0 + unit_id_);
        }
        glBindTexture(GL_TEXTURE_2D, handle_);
    }

    void SetUnitID(int unit_id) {
        unit_id_ = unit_id;
        Active();
    }

    void LoadFromFile(const std::string &file_path,
                      GLenum device_format = GL_RGB,
                      GLenum host_format = GL_RGB,
                      GLenum host_data_type = GL_UNSIGNED_BYTE);

    void LoadFromBuffer(void *source,
                        int width,
                        int height,
                        GLenum device_format = GL_RGB,
                        GLenum host_format = GL_RGB,
                        GLenum host_data_type = GL_UNSIGNED_BYTE) {
        width_ = width;
        height_ = height;
        device_format_ = device_format;
        host_format_ = host_format;
        host_data_type_ = host_data_type;
        glBindTexture(GL_TEXTURE_2D, handle_);
        glTexImage2D(GL_TEXTURE_2D, 0, device_format_, width_, height_, 0, host_format_, host_data_type_, source);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    /**
     * Resize will clear all the data.
     */
    void Resize(int width, int height) {
        LoadFromBuffer(NULL, width, height, device_format_, host_format_, host_data_type_);
    }

    GLuint GetHandle() {
        return handle_;
    }

private:
    unsigned int handle_;
    int unit_id_ = -1;
    GLenum device_format_;
    GLenum host_format_;
    GLenum host_data_type_;
    int width_;
    int height_;
};


#endif //GL_PLAYGROUND_GLTEXTURE2D_T_H
