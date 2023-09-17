//
// License: MIT
//

#include <memory>
#include <stdexcept>

#include <glpp/texture.h>

#include <stb_image.h>
namespace glpp {

void Texture2D::LoadFromFile(const std::string &file_path, GLenum device_format) {
  int width, height, channel;
  unsigned char *data = stbi_load(file_path.c_str(), &width, &height, &channel, 0);
  auto guard = std::shared_ptr<void>(data, [&](void *p) { stbi_image_free(p); });
  GLuint host_format = GL_RGB;
  switch (channel) {
  case 1:
    host_format = GL_RED;
    break;
  case 3:
    host_format = GL_RGB;
    break;
  case 4:
    host_format = GL_RGBA;
    break;
  default:
    throw std::runtime_error("unsupported image format");
  }
  LoadFromBuffer(data, width, height, device_format, host_format, GL_UNSIGNED_BYTE);
}
void Texture2D::LoadFromBuffer(void *data, int width, int height, GLenum device_format,
                               GLenum host_format, GLenum host_data_type) {
  Active();
  glTexImage2D(GL_TEXTURE_2D, 0, device_format, width, height, 0, host_format, host_data_type,
               data);
  glGenerateMipmap(GL_TEXTURE_2D);
}
} // namespace glpp