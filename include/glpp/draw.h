//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_DRAW_H
#define GLPP_INCLUDE_GLPP_DRAW_H
#include <glpp/shader_program.h>
#include <glpp/vertex_array.h>
namespace glpp::draw {

template <class VertexT>
void DrawVertexArray(const ShaderProgram &shder_program, const VertexArray<VertexT> &vertex_array,
                     GLenum mode = GL_TRIANGLES) {
  shder_program.Active();
  vertex_array.Active();
  glDrawArrays(mode, 0, vertex_array.GetBuffer().ByteSize() / sizeof(VertexT));
}
} // namespace glpp::draw
#endif // GLPP_INCLUDE_GLPP_DRAW_H
