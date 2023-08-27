//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_VERTEX_ARRAY_H
#define GLPP_INCLUDE_GLPP_VERTEX_ARRAY_H

#include <type_traits>

#include <glpp/buffer.h>
#include <glpp/common.h>

namespace glpp {
/**
 * VertexArray is logically a group of attributes of a vertex and corresponding data..
 * @tparam VertexT
 */
template <class VertexT> class VertexArray : public UnCopyable {
public:
  explicit VertexArray(const std::vector<VertexT> vertex_data = {})
      : vertex_buffer_(BufferType::ArrayBuffer) {
    static_assert(std::is_pod_v<VertexT>, "VertexT must be POD type");
    glGenVertexArrays(1, &handle_);
    glBindVertexArray(handle_);
    // Note that after register some attributes, the vertex buffer must be active.
    // and the vertex buffer will be bind to the VertexArray, so Active the vertex array will
    // active the vertex buffer automatically.
    vertex_buffer_.Active();
    VertexT::Describe();
    if (!vertex_data.empty()) {
      From(vertex_data);
    }
  }

  void From(const std::vector<VertexT> &data) { vertex_buffer_.From(data); }

  ~VertexArray() { glDeleteVertexArrays(1, &handle_); }

  void Active() const {
    glBindVertexArray(handle_); // Active the attribute
    vertex_buffer_.Active();    // Active the buffer
  }

  const Buffer &GetBuffer() const { return vertex_buffer_; }

  Buffer &GetBuffer() { return vertex_buffer_; }

private:
  GLuint handle_;
  Buffer vertex_buffer_;
};
} // namespace glpp
#endif // GLPP_INCLUDE_GLPP_VERTEX_ARRAY_H
