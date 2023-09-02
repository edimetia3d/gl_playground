//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_UI_PRESET_FPS_CAMERA_H
#define GLPP_INCLUDE_GLPP_UI_PRESET_FPS_CAMERA_H

#include "glpp/ui/preset/fps_io.h"

namespace glpp::ui {
// It will read camera position from IOHandler, and build a view box transform.
class FPSCamera {
public:
  FPSCamera(FPSIOHandler *handler, float width, float height, float fov, float near_dist,
            float far_dist)
      : width_(width), height_(height), fov_(fov), near_dist_(near_dist), far_dist_(far_dist),
        handler_(handler) {}

  void EnablePerspective(bool enable) { use_perspective_ = enable; }

  glm::mat4 GetViewBoxTransform();

private:
  float width_ = 800;
  float height_ = 600;
  float fov_ = 45;

  float near_dist_ = 0.1f;
  float far_dist_ = 100.0f;
  bool use_perspective_ = true;

  FPSIOHandler *handler_;
};
} // namespace glpp::ui

#endif // GLPP_INCLUDE_GLPP_UI_PRESET_FPS_CAMERA_H
