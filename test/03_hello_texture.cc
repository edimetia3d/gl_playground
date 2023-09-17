//
// License: MIT
//

#include <glpp/draw.h>
#include <glpp/shader_program.h>
#include <glpp/texture.h>
#include <glpp/vertex_array.h>

#include "test_resource.h"

struct PointWithTexturePos {
  float x;
  float y;
  float z;
  float texture_x;
  float texture_y;

  static void Describe() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointWithTexturePos),
                          (void *)offsetof(PointWithTexturePos, x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(PointWithTexturePos),
                          (void *)offsetof(PointWithTexturePos, texture_x));
    glEnableVertexAttribArray(1);
  }
};

struct TexturedTriangle {
  void Init() {
    vtx.From(vertices_data);
    program.Compile({vertex_shader}, {frag_shader});
    texture.LoadFromFile(TestRes("tile.jpg"));

    int texture_unit = 6;
    texture.ActiveOn(texture_unit);
    program.SetUniform("tile_texture", texture_unit);
  }

  void Display() {
    texture.Active();
    glpp::draw::DrawVertexArray(program, vtx);
  }

  glpp::ShaderProgram program;
  glpp::VertexArray<PointWithTexturePos> vtx;
  std::vector<PointWithTexturePos> vertices_data = {{-0.5f, -0.5f, 0.0f, 0.0f, 0.0f},
                                                    {0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
                                                    {0.0f, 0.5f, 0.0f, 0.5f, 1.0f}};
  glpp::Texture2D texture;
  const char *vertex_shader = R"(
#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_texture_pos;
out vec2 texture_pos;
void main()
{
    gl_Position = vec4(in_pos.x, in_pos.y, in_pos.z, 1.0f);
    texture_pos = vec2(in_texture_pos.x, 1.0f - in_texture_pos.y); // texture uses a different coordinate system
}
)";

  const char *frag_shader = R"(
#version 330 core
in vec2 texture_pos;
uniform sampler2D tile_texture;
out vec4 frag_color;
void main()
{
    frag_color = texture(tile_texture, texture_pos);
}
)";
};

int main() {
  glpp::ui::ExGFLWWindow window("hello texture", 4, 2);
  TexturedTriangle triangle;
  triangle.Init();
  auto render_fun = [&triangle](glpp::ui::ExGFLWWindow::RenderFunArg *arg) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    triangle.Display();
  };

  window.Show(render_fun, nullptr);
  return 0;
}
