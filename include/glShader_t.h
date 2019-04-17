//
// Created by edimetia3d on 2019-03-26.
//

#ifndef GL_PLAYGROUND_GLSHADER_T_H
#define GL_PLAYGROUND_GLSHADER_T_H

#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>

#include <glad/glad.h>

#include "UnCopyable.h"
#include "glTransform_t.h"

template<GLenum SHADER_TYPE>
class glShader_t : public UnCopyable {
public:
    glShader_t() {
        handle_ = glCreateShader(SHADER_TYPE);
    }

    ~glShader_t() {
        glDeleteShader(handle_);
    }

    bool Compile(const std::vector<std::string> &source_codes) {
        std::vector<const char *> codes_array(source_codes.size(), nullptr);
        for (int i = 0; i < source_codes.size(); ++i) {
            codes_array[i] = source_codes[i].c_str();
        }
        glShaderSource(handle_, codes_array.size(), &codes_array[0], NULL);
        glCompileShader(handle_);

        int success;
        char infoLog[512];
        glGetShaderiv(handle_, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(handle_, 512, NULL, infoLog);
            printf("SHADER COMPILE FAILED\n");
        }
        return success;
    }

    GLuint GetHandle() {
        return handle_;
    }

private:
    GLuint handle_;
};

using VertexShader_t=glShader_t<GL_VERTEX_SHADER>;
using FragmentShader_t=glShader_t<GL_FRAGMENT_SHADER>;

class glShaderProgram_t : public UnCopyable {
public:
    glShaderProgram_t() {
        handle_ = glCreateProgram();
    }

    ~glShaderProgram_t() {
        glDeleteProgram(handle_);
    }

    void Active() {
        glUseProgram(handle_);
    }

    bool Link(VertexShader_t &vtx, FragmentShader_t &frag) {
        glAttachShader(handle_, vtx.GetHandle());
        glAttachShader(handle_, frag.GetHandle());
        glLinkProgram(handle_);

        int success;
        char infoLog[512];
        glGetProgramiv(handle_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(handle_, 512, NULL, infoLog);
            printf("Program Link Failed\n");
        }
        return success;
    }

    void InitByStr(const std::vector<std::string> &all_vtx_code, const std::vector<std::string> &all_frag_code) {
        VertexShader_t vtx;
        vtx.Compile(all_vtx_code);
        FragmentShader_t frag;
        frag.Compile(all_frag_code);
        Link(vtx, frag);
    }

    void InitByFile(const std::vector<std::string> &all_vtx_filepath, const std::vector<std::string> &all_frag_filepath) {

        std::vector<std::string> vtx_strings;
        std::vector<std::string> frag_strings;
        for(auto &vtx_filepath:all_vtx_filepath){
            std::ifstream vtx_file(vtx_filepath);
            std::string vtx_string((std::istreambuf_iterator<char>(vtx_file)),(std::istreambuf_iterator<char>()));
            vtx_strings.push_back(std::move(vtx_string));
        }

        for(auto & frag_filepath:all_frag_filepath){
            std::ifstream frag_file(frag_filepath);
            std::string frag_string((std::istreambuf_iterator<char>(frag_file)),
                                    (std::istreambuf_iterator<char>()));
            frag_strings.push_back(std::move(frag_string));
        }

        InitByStr(vtx_strings, frag_strings);

    }

    template<class T>
    void SetTexture(T &texture, std::string var_name_in_shader, int unit_id) {
        Active();
        glUniform1i(glGetUniformLocation(handle_, var_name_in_shader.c_str()), unit_id);
        texture.SetUnitID(unit_id);
    }

    void SetTransform(glTransform_t &transform, std::string var_name_in_shader) {
        Active();
        glUniformMatrix4fv(glGetUniformLocation(handle_, var_name_in_shader.c_str()),
                           1,
                           GL_FALSE,
                           transform.Data());
    }

    GLuint GetHandle() {
        return handle_;
    }

private:
    GLuint handle_;
};

#endif //GL_PLAYGROUND_GLSHADER_T_H
