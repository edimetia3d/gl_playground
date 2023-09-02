//
// License: MIT
//

#include "glpp/ui/preset/fps_camera.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

glm::mat4 glpp::ui::FPSCamera::GetViewBoxTransform() {
  auto [pos, front, right] = handler_->GetNow();
  auto up = glm::cross(right, front);
  auto look_at = glm::lookAt(pos, pos + front, up);
  if (use_perspective_) {
    return glm::perspective(glm::radians(fov_), width_ / height_, near_dist_, far_dist_) * look_at;
  } else {
    return glm::ortho(-width_ / 2, width_ / 2, -height_ / 2, height_ / 2, near_dist_, far_dist_) *
           look_at;
  }
}
