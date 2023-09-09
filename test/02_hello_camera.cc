//
// License: MIT
//

#include <glpp/ui/preset/fps_camera.h>

#define AS_HEADER
#include "01_hello_triangle.cc"

const char *new_vertex_shader = R"(
#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 9) in vec3 in_color;
out vec3 color;

uniform mat4 look_trans;

void main()
{
    gl_Position = look_trans *  vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);
    color = in_color;
}
)";

int main() {
  glpp::ui::ExGFLWWindow window("hello camera", 4, 2);
  SimpleTriangle triangle;
  triangle.vertex_shader = new_vertex_shader;
  triangle.Init();
  glpp::ui::FPSIOHandler handler;
  auto [width, height] = window.WidthHeight();
  glpp::ui::FPSCamera camera(&handler, width, height, 45.0, 0.1f, 100.0f);
  auto render_fun = [&triangle, &camera](glpp::ui::ExGFLWWindow::RenderFunArg *arg) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    auto view = camera.GetViewBoxTransform();
    triangle.program.SetUniform("look_trans", view);
    triangle.Display();
  };

  window.Show(render_fun, &handler);
  return 0;
}