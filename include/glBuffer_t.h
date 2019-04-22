//
// Created by edimetia3d on 2019-03-26.
//

#ifndef GLPLAYGROUND_GLARRAY_T_H
#define GLPLAYGROUND_GLARRAY_T_H

#include <vector>

#include <glad/glad.h>
#include "UnCopyable.h"

/**
 * User must specialize this tempalte
 * @tparam Element_t
 */
template<class Element_t>
struct DefineVertexAttrib {
    static void Define();
};

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

    /**
     * Only this operation will alloc(or re-alloc) mem in device
     */
    void SyncToDevice(GLenum memory_type = GL_STATIC_DRAW) {
        Active();
        glBufferData(BUFFER_TYPE, host_buffer_.size() * sizeof(Element_t), host_buffer_.data(), memory_type);
    }

    template<class T>
    void CopyDataTo(T &another_glbuffer_t_obj, int64_t read_offset_byte = 0, int64_t write_offset_byte = 0,
                    int64_t cpy_size_byte = -1) {
        if (cpy_size_byte == -1) {
            cpy_size_byte = sizeof(Element_t) * host_buffer_.size();
        }
        glBindBuffer(GL_COPY_READ_BUFFER, handle_);
        glBindBuffer(GL_COPY_WRITE_BUFFER, another_glbuffer_t_obj.GetHandle());
        glCopyBufferSubData(GL_COPY_READ_BUFFER,
                            GL_COPY_WRITE_BUFFER,
                            read_offset_byte,
                            write_offset_byte,
                            cpy_size_byte);
    }

    void UpdateDeviceData(std::vector<Element_t> &buf, int byte_offset) {
        Active();
        glBufferSubData(BUFFER_TYPE, byte_offset, buf.size() * sizeof(Element_t), buf.data());
    }

    Element_t *GetMappedPointer(GLenum io_type = GL_READ_WRITE) {
        Active();
        return static_cast<Element_t *>(glMapBuffer(BUFFER_TYPE, io_type));
    }

    void ReleaseMappedPointer() {
        Active();
        glUnmapBuffer(BUFFER_TYPE);
    }

    std::vector<Element_t> &GetHostBuffer() {
        return host_buffer_;
    }

    int Size() {
        return host_buffer_.size();
    }

    GLuint GetHandle() {
        return handle_;
    }

    void DescribeData() {
        Active();
        DefineVertexAttrib<Element_t>::Define();
    }

private:
    std::vector<Element_t> host_buffer_;
    GLuint handle_;
};

template<class Element_t>
using glArrayBuffer_t=glBuffer_t<Element_t, GL_ARRAY_BUFFER>;

using glElementBuffer_t=glBuffer_t<GLuint, GL_ELEMENT_ARRAY_BUFFER>;

#endif //GLPLAYGROUND_GLARRAY_T_H

