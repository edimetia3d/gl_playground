//
// Created by edimetia3d on 2019-03-26.
//

#ifndef GLPLAYGROUND_GLARRAY_T_H
#define GLPLAYGROUND_GLARRAY_T_H

#include <vector>

#include <glad/glad.h>
#include "UnCopyable.h"

template<class Element_t, GLenum BUFFER_TYPE>
class glBuffer_t : public UnCopyable {
public:
    using value_type=Element_t;

    glBuffer_t() {
        glGenBuffers(1, &handle_);
    }

    ~glBuffer_t() {
        glDeleteBuffers(1, &handle_);
    }

    void Active() {
        glBindBuffer(BUFFER_TYPE, handle_);
    }

    void SyncToDevice(GLenum memory_type = GL_STATIC_DRAW) {
        Active();
        glBufferData(BUFFER_TYPE, host_buffer_.size() * sizeof(Element_t), host_buffer_.data(), memory_type);
    }

    std::vector<Element_t> &GetHostBuffer() {
        return host_buffer_;
    }

    int Size() {
        return host_buffer_.size();
    }

private:
    std::vector<Element_t> host_buffer_;
    GLuint handle_;
};

template<class Element_t>
using glArrayBuffer_t=glBuffer_t<Element_t, GL_ARRAY_BUFFER>;

using glElementBuffer_t=glBuffer_t<GLuint, GL_ELEMENT_ARRAY_BUFFER>;

#endif //GLPLAYGROUND_GLARRAY_T_H

