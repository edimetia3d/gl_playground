//
// Created by edimetia3d on 2019-04-07.
//

#ifndef GL_PLAYGROUND_ASSIMPMESH_T_H
#define GL_PLAYGROUND_ASSIMPMESH_T_H

#include "glVertexArray_t.h"
#include "glTexture2D_t.h"
#include "glShader_t.h"
#include <vector>

struct AssimpVertex {
    struct Vec3 {
        float x;
        float y;
        float z;
    };

    struct Vec2 {
        float x;
        float y;
    };

    // position
    Vec3 position;
    // normal
    Vec3 normal;
    // texCoords
    Vec2 texCoords;
    // tangent
    Vec3 tangent;
    // bitangent
    Vec3 bitangent;
};

template<>
struct DefineVertexAttrib<AssimpVertex> {
    static void Define() {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex),
                              (void *) offsetof(AssimpVertex, position));
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex),
                              (void *) offsetof(AssimpVertex, normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex),
                              (void *) offsetof(AssimpVertex, texCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex),
                              (void *) offsetof(AssimpVertex, tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpVertex),
                              (void *) offsetof(AssimpVertex, bitangent));
    }
};

enum class TextureType {
    DIFFUSE,
    SPECULAR,
    NORMAL,
    HEIGHT
};

std::string GetNameString(TextureType type) {
    switch (type) {
        case TextureType::DIFFUSE :
            return "texture_diffuse";
        case TextureType::SPECULAR:
            return "texture_specular";
        case TextureType::NORMAL:
            return "texture_normal";
        case TextureType::HEIGHT:
            return "texture_height";
        default:
            return "";
    }

}

class AssimpMesh_t {
public:
    void Draw(glShaderProgram_t &the_shader, const std::vector<std::string> &texture_names = {}) {
        auto p_to_name = texture_names_.cbegin();
        if (!texture_names.empty()) {
            p_to_name = texture_names.cbegin();
        }
        for (unsigned int i = 0; i < textures_.size(); i++) {
            the_shader.SetTexture(*textures_[i], *p_to_name, i);
            textures_[i]->Active();
            ++p_to_name;
        }
        the_shader.Active();
        vertex_.DrawByIndex(GL_TRIANGLES);
    }

    void PushNewTexture(std::shared_ptr<glTexture2D_t> texture, TextureType type) {
        std::string name_in_shader;
        switch (type) {
            case TextureType::DIFFUSE :
                name_in_shader = GetNameString(type) + std::to_string(diffuse_last_++);
                break;
            case TextureType::SPECULAR:
                name_in_shader = GetNameString(type) + std::to_string(specular_last_++);
                break;
            case TextureType::NORMAL:
                name_in_shader = GetNameString(type) + std::to_string(normal_last_++);
                break;
            case TextureType::HEIGHT:
                name_in_shader = GetNameString(type) + std::to_string(height_last_++);
                break;
        }
        textures_.push_back(texture);
        texture_names_.push_back(name_in_shader);
    }

    glVertexArray_t<AssimpVertex> vertex_;

private:
    std::vector<std::shared_ptr<glTexture2D_t>> textures_;
    std::vector<std::string> texture_names_;
    unsigned int diffuse_last_ = 1;
    unsigned int specular_last_ = 1;
    unsigned int normal_last_ = 1;
    unsigned int height_last_ = 1;
};


#endif //GL_PLAYGROUND_ASSIMPMESH_T_H
