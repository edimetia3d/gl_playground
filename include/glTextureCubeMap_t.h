//
// Created by edimetia3d on 2019-04-15.
//

#ifndef GL_PLAYGROUND_GLTEXTURECUBEMAP_T_H
#define GL_PLAYGROUND_GLTEXTURECUBEMAP_T_H

#include <string>
#include <vector>

#include <glad/glad.h>

#include "UnCopyable.h"

class glTextureCubeMap_t : public UnCopyable {
public:
    glTextureCubeMap_t() {
        glGenTextures(1, &handle_);
    }

    ~glTextureCubeMap_t() {
        glDeleteTextures(1, &handle_);
    }

    void Active() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, handle_);
    }

    void SetUnitID(int unit_id) {
        unit_id_ = unit_id;
        glActiveTexture(GL_TEXTURE0 + unit_id_);
        Active();
    }

    void LoadFromFile(const std::vector<std::string> &filenames,
                      GLenum device_format = GL_RGB,
                      GLenum host_format = GL_RGB,
                      GLenum host_data_type = GL_UNSIGNED_BYTE);

    void LoadFromBuffer(const std::vector<void *> &sources,
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
        Active();
        for (unsigned int i = 0; i < sources.size(); i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, device_format_, width_, height_, 0, host_format_,
                         host_data_type_, sources[i]);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }


private:
    unsigned int handle_;
    GLenum device_format_;
    GLenum host_format_;
    GLenum host_data_type_;
    int width_;
    int height_;
    int unit_id_ = 0;
};


#endif //GL_PLAYGROUND_GLTEXTURECUBEMAP_T_H
