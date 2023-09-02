//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_UI_PRESET_FPS_IO_H
#define GLPP_INCLUDE_GLPP_UI_PRESET_FPS_IO_H

#include "glpp/ui/io_handler.h"

#include <glm/glm.hpp>

/**
 * A FPS style IO handler, it will use IO to update camera position and direction.
 * 1. WSAD to move camera.
 * 2. mouse to rotate camera.
 * 3. press ESC to exit.
 * 4. hold CTRL to disable mouse.
 */
namespace glpp::ui {
class FPSIOHandler : public IOHandler {
public:
  FPSIOHandler();
  ~FPSIOHandler() = default;

  void OnMouseMove(double x, double y, double dx, double dy) override;

  // A tuple of (camera_pos, camera_direction,camera_right)
  std::tuple<glm::vec3, glm::vec3, glm::vec3> GetNow();

private:
  // Camera
  glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
  double camera_speed_tic[2] = {0.0, 0.0};  // the time of when speed is updated
  double camera_speed_mask[2] = {0.0, 0.0}; // the mask of forward/backward and left/right
  float camera_yaw = -90.0f;
  float camera_pitch = 0.0f;

  // Sensitivity
  float rotate_sensitivity_ = 0.1f;
  float move_sensitivity_ = 2.0f;
};
} // namespace glpp::ui
#endif // GLPP_INCLUDE_GLPP_UI_PRESET_FPS_IO_H
