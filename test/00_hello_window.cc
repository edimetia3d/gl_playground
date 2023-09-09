//
// License: MIT
//
#include "glpp/ui/ui_window.h"


void RenderNothing(glpp::ui::ExGFLWWindow::RenderFunArg *arg) {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

int main() {
  glpp::ui::ExGFLWWindow window("hello window", 4, 2);
  window.Show(RenderNothing, nullptr);
  return 0;
}