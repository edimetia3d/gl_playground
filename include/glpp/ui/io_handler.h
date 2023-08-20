//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_UI_IO_HANDLER_H
#define GLPP_INCLUDE_GLPP_UI_IO_HANDLER_H
#include <functional>
#include <map>

namespace glpp::ui {
class IOHandler {
public:
  using KeyHandler = std::function<void(int scancode, int modifier)>;
  virtual void OnMouseMove(double x, double y, double dx, double dy) {}
  void RegOnPress(int key, KeyHandler handler);
  void RegOnRelease(int key, KeyHandler handler);

  std::map<int, KeyHandler> &OnPressRegistry() { return on_press_; }
  std::map<int, KeyHandler> &OnReleaseRegistry() { return on_release_; }

private:
  std::map<int, KeyHandler> on_press_;
  std::map<int, KeyHandler> on_release_;
};
} // namespace glpp::ui
#endif // GLPP_INCLUDE_GLPP_UI_IO_HANDLER_H
