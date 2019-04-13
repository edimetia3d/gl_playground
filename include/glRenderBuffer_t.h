//
// Created by edimetia3d on 2019-04-13.
//

#ifndef GL_PLAYGROUND_GLRENDERBUFFER_T_H
#define GL_PLAYGROUND_GLRENDERBUFFER_T_H

#include <glad/glad.h>
#include "UnCopyable.h"

class glRenderBuffer_t {
public:
    glRenderBuffer_t(GLenum device_type, int width = 800, int height = 600)
            : device_type_(device_type),
              width_(width),
              height_(height) {
        glGenRenderbuffers(1, &handle_);
        glRenderbufferStorage(GL_RENDERBUFFER, device_type_, width_, height_);
    }

    ~glRenderBuffer_t() {
        glDeleteRenderbuffers(1, &handle_);
    }

    void Active() {
        glBindRenderbuffer(GL_RENDERBUFFER, handle_);
    }

    void Resize(int width, int height) {
        width_ = width;
        height_ = height;
        Active();
        glRenderbufferStorage(GL_RENDERBUFFER, device_type_, width_, height_);
    }

    GLuint GetHandle() {
        return handle_;
    }

private:
    GLuint handle_;
    int width_;
    int height_;
    GLenum device_type_;
};


#endif //GL_PLAYGROUND_GLRENDERBUFFER_T_H
