//
// Created by edimetia3d on 2019-04-15.
//

#include "include/glTextureCubeMap_t.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include "dependences/stb_image.h"

void
glTextureCubeMap_t::LoadFromFile(const std::vector<std::string> &filenames, GLenum device_format, GLenum host_format,
                                 GLenum host_data_type) {
    int width, height, nrChannels;
    std::vector<void *> sources;
    for (int i = 0; i < filenames.size(); ++i) {
        unsigned char *data = stbi_load(filenames[i].c_str(), &width, &height, &nrChannels, 0);
        if (host_format == GL_RGB && nrChannels != 3) {
            if (nrChannels == 1) {
                host_format = GL_RED;
            }
            if (nrChannels == 4) {
                host_format = GL_RGBA;
            }
        }
        sources.push_back(data);
    }
    LoadFromBuffer(sources, width, height, device_format, host_format, host_data_type);
    for (auto data:sources) {
        stbi_image_free(data);
    }
}