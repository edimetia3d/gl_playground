//
// License: MIT
//

#include "glpp/ui/io_handler.h"

namespace glpp::ui {

void ui::IOHandler::RegOnPress(int key, IOHandler::KeyHandler handler) { on_press_[key] = handler; }
void IOHandler::RegOnRelease(int key, IOHandler::KeyHandler handler) { on_release_[key] = handler; }
} // namespace glpp::ui