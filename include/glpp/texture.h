//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_TEXTURE_H
#define GLPP_INCLUDE_GLPP_TEXTURE_H

#include <string>
#include <vector>

#include <glpp/common.h>
#include <glpp/ui/ui_window.h>

namespace glpp {

class Texture2D : public UnCopyable {
public:
  Texture2D() { glGenTextures(1, &handle_); }

  ~Texture2D() { glDeleteTextures(1, &handle_); }

  void Active() { glBindTexture(GL_TEXTURE_2D, handle_); }

  void ActiveOn(GLuint texture_unit) {
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    Active();
  }

  void LoadFromFile(const std::string &file_path, GLenum device_format = GL_RGB);

  void LoadFromBuffer(void *data, int width, int height, GLenum device_format = GL_RGB,
                      GLenum host_format = GL_RGB, GLenum host_data_type = GL_UNSIGNED_BYTE);

  GLuint GetHandle() { return handle_; }

private:
  GLuint handle_;
};

} // namespace glpp

#endif // GLPP_INCLUDE_GLPP_TEXTURE_H
