//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_UI_IO_HANDLER_H
#define GLPP_INCLUDE_GLPP_UI_IO_HANDLER_H

#include <functional>
#include <map>

namespace glpp::ui {
class ExGFLWWindow;

class IOHandler {
public:
  using KeyHandler = std::function<void(int scancode, int modifier)>;
  virtual void OnMouseMove(double x, double y, double dx, double dy) {}
  virtual bool ShouldHideCursor() { return false; }

  std::map<int, KeyHandler> &OnPressRegistry() { return on_press_; }
  std::map<int, KeyHandler> &OnReleaseRegistry() { return on_release_; }
  std::map<int, KeyHandler> &OnRepeatRegistry() { return on_repeat_; }

protected:
  friend class ExGFLWWindow;
  void BindWindow(ExGFLWWindow *window) { window_ = window; }
  std::map<int, KeyHandler> on_press_;
  std::map<int, KeyHandler> on_repeat_;
  std::map<int, KeyHandler> on_release_;
  ExGFLWWindow *window_ = nullptr;
};
} // namespace glpp::ui
#endif // GLPP_INCLUDE_GLPP_UI_IO_HANDLER_H
