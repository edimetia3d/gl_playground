//
// License: MIT
//

#include <glpp/draw.h>
#include <glpp/shader_program.h>
#include <glpp/vertex_array.h>

struct PointWithColor {
  float x;
  float y;
  float z;
  float r;
  float g;
  float b;

  static void Describe() {
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PointWithColor),
                          (void *)offsetof(PointWithColor, x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(PointWithColor),
                          (void *)offsetof(PointWithColor, r));
    glEnableVertexAttribArray(9);
  }
};

struct SimpleTriangle {
  void Init() {
    vtx.From(vertices_data);
    program.Compile({vertex_shader}, {frag_shader});
  }

  void Display() { glpp::draw::DrawVertexArray(program, vtx); }

  glpp::ShaderProgram program;
  glpp::VertexArray<PointWithColor> vtx;
  std::vector<PointWithColor> vertices_data = {{-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f},
                                               {0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f},
                                               {0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f}};
  const char *vertex_shader = "#version 330 core\n"
                              "layout (location = 0) in vec3 aPos;\n"
                              "layout (location = 9) in vec3 aColor;"
                              "out vec3 color;"
                              "void main()\n"
                              "{\n"
                              "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                              "   color=aColor;"
                              "}\0";

  const char *frag_shader = "#version 330 core\n"
                            "in vec3 color;"
                            "out vec4 FragColor;\n"
                            "void main()\n"
                            "{\n"
                            "   FragColor = vec4(color, 1.0f);\n"
                            "}\n\0";
};

#ifndef AS_HEADER
int main() {
  glpp::ui::ExGFLWWindow window("hello triangle", 4, 2);
  SimpleTriangle triangle;
  triangle.Init();
  auto render_fun = [&triangle](glpp::ui::ExGFLWWindow::RenderFunArg *arg) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    triangle.Display();
  };

  window.Show(render_fun, nullptr);
  return 0;
}
#endif