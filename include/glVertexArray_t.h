//
// Created by edimetia3d on 2019-03-26.
//

#ifndef GL_PLAYGROUND_GLVERTEXBUFFER_T_H
#define GL_PLAYGROUND_GLVERTEXBUFFER_T_H

#include "glBuffer_t.h"



template<class Element_t>
class glVertexArray_t : public UnCopyable {
public:
    glVertexArray_t() {
        glGenVertexArrays(1, &handle_);
        glBindVertexArray(handle_);
        vertex_buffer_.Active();
        index_buffer_.Active();
        vertex_buffer_.DescribeData();
        glBindVertexArray(0);
    }

    ~glVertexArray_t() {
        glDeleteVertexArrays(1, &handle_);
    }

    void Active() {
        glBindVertexArray(handle_);
    }

    void SyncToDevice(GLenum memory_type = GL_STATIC_DRAW) {
        vertex_buffer_.SyncToDevice(memory_type);
        index_buffer_.SyncToDevice(memory_type);
    }

    std::vector<Element_t> &GetVertexBuffer() {
        return vertex_buffer_.GetHostBuffer();
    }

    std::vector<GLuint> &GetIndexBuffer() {
        return index_buffer_.GetHostBuffer();
    }

    void DrawByIndex(GLenum draw_type, int index_num_to_use = -1) {
        Active();
        if (index_num_to_use == -1) {
            index_num_to_use = index_buffer_.Size();
        }
        index_num_to_use=std::min(index_num_to_use,index_buffer_.Size());
        if(index_num_to_use==0){
            glDrawArrays(draw_type,0,vertex_buffer_.Size());
        }
        else{
            glDrawElements(draw_type, index_num_to_use, GL_UNSIGNED_INT, 0);
        }
    }

    template<class T>
    void AddInstanceBuffer(T &buffer) {
        Active();
        buffer.DescribeData();
    }

    void DrawInstanceByIndex(GLenum draw_type, int instance_num = 1, int index_num_to_use = -1) {
        Active();
        if (index_num_to_use == -1) {
            index_num_to_use = index_buffer_.Size();
        }
        index_num_to_use = std::min(index_num_to_use, index_buffer_.Size());
        if (index_num_to_use == 0) {
            glDrawArraysInstanced(draw_type, 0, vertex_buffer_.Size(), instance_num);
        } else {
            glDrawElementsInstanced(draw_type, index_num_to_use, GL_UNSIGNED_INT, 0, instance_num);
        }
    }

private:
    GLuint handle_;
    glArrayBuffer_t<Element_t> vertex_buffer_;
    glElementBuffer_t index_buffer_;
};

#endif //GL_PLAYGROUND_GLVERTEXBUFFER_T_H
