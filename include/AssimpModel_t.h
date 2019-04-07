//
// Created by edimetia3d on 2019-04-07.
//

#ifndef GL_PLAYGROUND_ASSIMPMODEL_T_H
#define GL_PLAYGROUND_ASSIMPMODEL_T_H

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AssimpMesh_t.h"

class AssimpModel {
public:
    void loadModel(std::string path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                                       aiProcess_CalcTangentSpace);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            printf("ERROR::ASSIMP:: %s \n ", importer.GetErrorString());
            return;
        }
        directory = path.substr(0, path.find_last_of('/'));
        processNode(scene->mRootNode, scene);
    }

    void Draw(glShaderProgram_t &shader) {
        for (auto &mesh:meshes) {
            mesh->Draw(shader);
        }
    }

private:
    std::vector<std::shared_ptr<AssimpMesh_t>> meshes;
    struct LoadedTexture {
        std::vector<std::shared_ptr<glTexture2D_t>> texture;
        std::vector<std::string> path;
        std::vector<TextureType> texture_type;
    } textures_loaded;
    std::string directory;

    void processNode(aiNode *node, const aiScene *scene) {
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    std::shared_ptr<AssimpMesh_t> processMesh(aiMesh *mesh, const aiScene *scene) {
        // data to fill
        auto ret = std::make_shared<AssimpMesh_t>();
        auto &vertices = ret->vertex_.GetVertexBuffer();
        auto &indices = ret->vertex_.GetIndexBuffer();

        // Walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            AssimpVertex vertex;
            // positions
            vertex.position.x = mesh->mVertices[i].x;
            vertex.position.y = mesh->mVertices[i].y;
            vertex.position.z = mesh->mVertices[i].z;
            // normals
            vertex.normal.x = mesh->mNormals[i].x;
            vertex.normal.y = mesh->mNormals[i].y;
            vertex.normal.z = mesh->mNormals[i].z;
            // texture coordinates
            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                vertex.texCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.texCoords.y = mesh->mTextureCoords[0][i].y;
            } else {
                vertex.texCoords.x = 0.0f;
                vertex.texCoords.y = 0.0f;
            }
            // tangent
            vertex.tangent.x = mesh->mTangents[i].x;
            vertex.tangent.y = mesh->mTangents[i].y;
            vertex.tangent.z = mesh->mTangents[i].z;
            // bitangent
            vertex.bitangent.x = mesh->mBitangents[i].x;
            vertex.bitangent.y = mesh->mBitangents[i].y;
            vertex.bitangent.z = mesh->mBitangents[i].z;
            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        ret->vertex_.SyncToDevice();
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DIFFUSE, ret);
        loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SPECULAR, ret);
        loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::NORMAL, ret);
        loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::HEIGHT, ret);

        return ret;
    }

    void loadMaterialTextures(aiMaterial *mat, aiTextureType assimp_type, TextureType type,
                              std::shared_ptr<AssimpMesh_t> target) {
        for (unsigned int i = 0; i < mat->GetTextureCount(assimp_type); i++) {
            aiString str;
            mat->GetTexture(assimp_type, i, &str);
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.texture.size(); j++) {
                if (std::strcmp(textures_loaded.path[j].data(), str.C_Str()) == 0) {
                    target->PushNewTexture(textures_loaded.texture[j], textures_loaded.texture_type[j]);
                    skip = true;
                    break;
                }
            }
            if (!skip) {
                auto new_texture = std::make_shared<glTexture2D_t>();
                new_texture->LoadFromFile(this->directory + "/" + str.C_Str());
                target->PushNewTexture(new_texture, type);
                textures_loaded.texture.push_back(new_texture);
                textures_loaded.path.push_back(str.C_Str());
                textures_loaded.texture_type.push_back(type);
            }
        }
    }
};


#endif //GL_PLAYGROUND_ASSIMPMODEL_T_H
