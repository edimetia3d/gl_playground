//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_BUFFER_H
#define GLPP_INCLUDE_GLPP_BUFFER_H

#include <memory>

#include <glpp/common.h>
#include <glpp/ui/ui_window.h>
namespace glpp {
enum class BufferType {
  VertexBuffer,
  IndexBuffer,
  UniformBuffer,
  ShaderStorageBuffer,
  AtomicCounterBuffer,
  PixelPackBuffer,
  PixelUnpackBuffer,
  TransformFeedbackBuffer,
  CopyReadBuffer,
  CopyWriteBuffer,
  DispatchIndirectBuffer,
  DrawIndirectBuffer,
  QueryBuffer,
  TextureBuffer,
  ArrayBuffer,
  ElementArrayBuffer
};

enum class BufferUsage {
  StaticDraw,
  DynamicDraw,
  StreamDraw,
  StaticRead,
  DynamicRead,
  StreamRead,
  StaticCopy,
  DynamicCopy,
  StreamCopy,
};

template <class T> class MappedDeviceBuffer : public UnCopyable {
public:
  T *Ptr() { return static_cast<T *>(ptr_); }

  int64_t Length() { return byte_size_ / sizeof(T); }

  T &operator[](int64_t index) { return Ptr()[index]; }

  ~MappedDeviceBuffer() {
    glBindBuffer(buffer_type_, handle_);
    glUnmapBuffer(buffer_type_);
  }

private:
  friend class Buffer;
  MappedDeviceBuffer(GLenum buffer_type, GLuint handle, void *ptr, int64_t byte_size)
      : buffer_type_(buffer_type), handle_(handle), ptr_(ptr), byte_size_(byte_size) {}
  GLenum buffer_type_;
  int64_t byte_size_;
  GLuint handle_;
  void *ptr_;
};

class Buffer : public UnCopyable {
public:
  explicit Buffer(BufferType type);

  ~Buffer() { glDeleteBuffers(1, &handle_); }

  void Active() const;

  template <class T>
  void From(const std::vector<T> &data, BufferUsage usage = BufferUsage::StaticDraw) {
    From(data.data(), data.data() + data.size(), usage);
  }

  void From(const void *beg, const void *end, BufferUsage usage = BufferUsage::StaticDraw);

  void Substitue(const void *beg, const void *end, int64_t offset_byte = 0);

  template <class T> std::unique_ptr<MappedDeviceBuffer<T>> MapToHost();

  int64_t ByteSize() const { return byte_size_; }

  GLuint GetHandle() const { return handle_; }

private:
  GLenum buffer_type_;
  int64_t byte_size_;
  GLuint handle_;
};

template <class T> std::unique_ptr<MappedDeviceBuffer<T>> Buffer::MapToHost() {
  Active();
  auto ptr = glMapBuffer(buffer_type_, GL_READ_WRITE);
  return std::make_unique<MappedDeviceBuffer<T>>(buffer_type_, handle_, ptr, byte_size_);
}

} // namespace glpp

#endif // GLPP_INCLUDE_GLPP_BUFFER_H
