//
// Created by edimetia3d on 2019-04-13.
//

#ifndef GL_PLAYGROUND_GLFRAMEBUFFER_T_H
#define GL_PLAYGROUND_GLFRAMEBUFFER_T_H

#include <memory>
#include <vector>
#include <map>

#include <glad/glad.h>

#include "UnCopyable.h"
#include "glTexture2D_t.h"
#include "glRenderBuffer_t.h"

enum class ContainerFormat {
    TEXTURE_2D,
    RenderBuffer
};

template<GLenum FRAMEBUFFER_TARGET = GL_FRAMEBUFFER>
class glFrameBuffer_t : public UnCopyable {
public:
    glFrameBuffer_t() {
        glGenFramebuffers(1, &handel_);
    }

    void Init(int width,
              int height,
              ContainerFormat color_format = ContainerFormat::RenderBuffer,
              ContainerFormat depth_stencil_format = ContainerFormat::RenderBuffer) {
        if (!inited_) {
            Active();
            width_ = width;
            height_ = height;
            AddAttachment<GL_COLOR_ATTACHMENT0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE>(color_format);
            AddAttachment<GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8>(
                    depth_stencil_format);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                printf("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
            }
            inited_ = true;
        }

    }

    ~glFrameBuffer_t() {
        glDeleteFramebuffers(1, &handel_);
    }

    void Active() {
        glBindFramebuffer(FRAMEBUFFER_TARGET, handel_);
    }

    std::shared_ptr<glTexture2D_t> GetTexuture(GLenum attach_point = GL_COLOR_ATTACHMENT0) {
        if (attach_point_to_texture.find(attach_point) == attach_point_to_texture.end()) {
            return nullptr;
        }
        return attach_point_to_texture[attach_point];
    }

    void LazyResize(int width, int height) {
        if (width != width_ || height != height_) {
            width_ = width;
            height_ = height;
            for (auto &p:render_buffers) {
                p->Resize(width_, height_);
            }
            for (auto &p:texture_buffers) {
                p->Resize(width_, height_);
            }
        }
    }

private:
    template<GLenum ATTACH_POINT, GLenum DEVICE_FORMAT, GLenum HOST_FORMAT, GLenum HOST_DATA_TYPE>
    void AddAttachment(ContainerFormat color_format) {
        switch (color_format) {
            case ContainerFormat::TEXTURE_2D : {
                auto p = std::make_shared<glTexture2D_t>();
                p->LoadFromBuffer(nullptr, width_, height_, DEVICE_FORMAT, HOST_FORMAT, HOST_DATA_TYPE);
                glFramebufferTexture2D(FRAMEBUFFER_TARGET, ATTACH_POINT, GL_TEXTURE_2D, p->GetHandle(), 0);
                texture_buffers.push_back(p);
                attach_point_to_texture[ATTACH_POINT] = p;
                break;
            }
            case ContainerFormat::RenderBuffer : {
                auto p = std::make_shared<glRenderBuffer_t>();
                p->Init(DEVICE_FORMAT, width_, height_);
                glFramebufferRenderbuffer(FRAMEBUFFER_TARGET, ATTACH_POINT, GL_RENDERBUFFER, p->GetHandle());
                render_buffers.push_back(p);
                break;
            }
            default:
                // should never goes here
                throw;
        }

    }

    std::vector<std::shared_ptr<glRenderBuffer_t>> render_buffers;
    std::vector<std::shared_ptr<glTexture2D_t>> texture_buffers;
    int width_;
    int height_;
    bool inited_ = false;
    GLuint handel_;
    std::map<GLenum, std::shared_ptr<glTexture2D_t>> attach_point_to_texture;
};

#endif //GL_PLAYGROUND_GLFRAMEBUFFER_T_H
