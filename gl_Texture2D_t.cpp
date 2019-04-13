//
// Created by edimetia3d on 2019-03-29.
//
#include "include/glTexture2D_t.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include "dependences/stb_image.h"


void glTexture2D_t::LoadFromFile(const std::string &file_path,
                                 GLenum device_format,
                                 GLenum host_format,
                                 GLenum host_data_type) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);
    if (host_format == GL_RGB && nrChannels != 3) {
        if (nrChannels == 1) {
            host_format = GL_RED;
        }
        if (nrChannels == 4) {
            host_format = GL_RGBA;
        }
    }
    LoadFromBuffer(data, width, height, device_format, host_format, host_data_type);
    stbi_image_free(data);
}

