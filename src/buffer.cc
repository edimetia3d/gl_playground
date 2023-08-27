//
// License: MIT
//

#include <glpp/buffer.h>

namespace glpp {

static GLenum ToGLBufferType(BufferType type) {
  switch (type) {
  case BufferType::VertexBuffer:
    return GL_ARRAY_BUFFER;
  case BufferType::IndexBuffer:
    return GL_ELEMENT_ARRAY_BUFFER;
  case BufferType::UniformBuffer:
    return GL_UNIFORM_BUFFER;
  case BufferType::ShaderStorageBuffer:
    return GL_SHADER_STORAGE_BUFFER;
  case BufferType::AtomicCounterBuffer:
    return GL_ATOMIC_COUNTER_BUFFER;
  case BufferType::PixelPackBuffer:
    return GL_PIXEL_PACK_BUFFER;
  case BufferType::PixelUnpackBuffer:
    return GL_PIXEL_UNPACK_BUFFER;
  case BufferType::TransformFeedbackBuffer:
    return GL_TRANSFORM_FEEDBACK_BUFFER;
  case BufferType::CopyReadBuffer:
    return GL_COPY_READ_BUFFER;
  case BufferType::CopyWriteBuffer:
    return GL_COPY_WRITE_BUFFER;
  case BufferType::DispatchIndirectBuffer:
    return GL_DISPATCH_INDIRECT_BUFFER;
  case BufferType::DrawIndirectBuffer:
    return GL_DRAW_INDIRECT_BUFFER;
  case BufferType::QueryBuffer:
    return GL_QUERY_BUFFER;
  case BufferType::TextureBuffer:
    return GL_TEXTURE_BUFFER;
  case BufferType::ArrayBuffer:
    return GL_ARRAY_BUFFER;
  case BufferType::ElementArrayBuffer:
    return GL_ELEMENT_ARRAY_BUFFER;
  default:
    return GL_ARRAY_BUFFER;
  }
}
void Buffer::Active() const { glBindBuffer(buffer_type_, handle_); }

static GLenum ToGlBufferUsage(BufferUsage usage) {
  switch (usage) {
  case BufferUsage::StaticDraw:
    return GL_STATIC_DRAW;
  case BufferUsage::DynamicDraw:
    return GL_DYNAMIC_DRAW;
  case BufferUsage::StreamDraw:
    return GL_STREAM_DRAW;
  case BufferUsage::StaticRead:
    return GL_STATIC_READ;
  case BufferUsage::DynamicRead:
    return GL_DYNAMIC_READ;
  case BufferUsage::StreamRead:
    return GL_STREAM_READ;
  case BufferUsage::StaticCopy:
    return GL_STATIC_COPY;
  case BufferUsage::DynamicCopy:
    return GL_DYNAMIC_COPY;
  case BufferUsage::StreamCopy:
    return GL_STREAM_COPY;
  default:
    return GL_STATIC_DRAW;
  }
}

void Buffer::From(const void *beg, const void *end, BufferUsage usage) {
  Active();
  byte_size_ = static_cast<const char *>(end) - static_cast<const char *>(beg);
  glBufferData(buffer_type_, byte_size_, beg, ToGlBufferUsage(usage));
}
Buffer::Buffer(BufferType type) {
  buffer_type_ = ToGLBufferType(type);
  glGenBuffers(1, &handle_);
}
void Buffer::Substitue(const void *beg, const void *end, int64_t offset_byte) {
  Active();
  glBufferSubData(buffer_type_, offset_byte,
                  static_cast<const char *>(end) - static_cast<const char *>(beg), beg);
}

} // namespace glpp
