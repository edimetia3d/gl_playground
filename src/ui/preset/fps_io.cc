//
// License: MIT
//

#include "glpp/ui/preset/fps_io.h"

#include "glpp/ui/ui_window.h"

#include <algorithm>

#include <GLFW/glfw3.h>

namespace glpp::ui {
void FPSIOHandler::OnMouseMove(double x, double y, double dx, double dy) {
  camera_yaw += dx * rotate_sensitivity_;
  camera_pitch += dy * rotate_sensitivity_;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  camera_pitch = std::clamp(camera_pitch, -89.9f, 89.9f);
}

FPSIOHandler::FPSIOHandler() {
  auto OnWPress = [this](int scancode, int modifier) {
    camera_speed_tic[0] = glfwGetTime();
    camera_speed_mask[0] = 1;
  };
  auto OnSPress = [this](int scancode, int modifier) {
    camera_speed_tic[0] = glfwGetTime();
    camera_speed_mask[0] = -1;
  };
  auto OnAPress = [this](int scancode, int modifier) {
    camera_speed_tic[1] = glfwGetTime();
    camera_speed_mask[1] = -1;
  };
  auto OnDPress = [this](int scancode, int modifier) {
    camera_speed_tic[1] = glfwGetTime();
    camera_speed_mask[1] = 1;
  };
  auto OnCtrlPress = [this](int scancode, int modifier) {
    glfwSetInputMode(window_->Handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  };
  auto OnCtrlRelease = [this](int scancode, int modifier) {
    glfwSetInputMode(window_->Handle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  };
  auto OnESCPress = [this](int scancode, int modifier) {
    glfwSetWindowShouldClose(window_->Handle(), true);
  };
  auto OnWSRelease = [this](int scancode, int modifier) { camera_speed_mask[0] = 0; };
  auto OnADRelease = [this](int scancode, int modifier) { camera_speed_mask[1] = 0; };

  OnPressRegistry()[GLFW_KEY_W] = OnWPress;
  OnPressRegistry()[GLFW_KEY_S] = OnSPress;
  OnPressRegistry()[GLFW_KEY_A] = OnAPress;
  OnPressRegistry()[GLFW_KEY_D] = OnDPress;
  OnPressRegistry()[GLFW_KEY_LEFT_CONTROL] = OnCtrlPress;
  OnPressRegistry()[GLFW_KEY_ESCAPE] = OnESCPress;

  OnReleaseRegistry()[GLFW_KEY_W] = OnWSRelease;
  OnReleaseRegistry()[GLFW_KEY_S] = OnWSRelease;
  OnReleaseRegistry()[GLFW_KEY_A] = OnADRelease;
  OnReleaseRegistry()[GLFW_KEY_D] = OnADRelease;
  OnReleaseRegistry()[GLFW_KEY_LEFT_CONTROL] = OnCtrlRelease;
}

std::tuple<glm::vec3, glm::vec3, glm::vec3> FPSIOHandler::GetNow() {
  static const glm::vec3 absolute_up = glm::vec3(0.0f, 1.0f, 0.0f);

  // update direction and right
  glm::vec3 front;
  double yaw = glm::radians(camera_yaw);
  double pitch = glm::radians(camera_pitch);
  front.x = cos(yaw) * cos(pitch);
  front.y = sin(pitch);
  front.z = sin(yaw) * cos(pitch);
  auto direction = glm::normalize(front);
  auto right = glm::cross(direction, absolute_up);

  // update position
  double current_time = glfwGetTime();
  glm::vec3 move_speed = {0, 0, 0};
  if (camera_speed_mask[0] != 0) {
    move_speed = (float)camera_speed_mask[0] * move_sensitivity_ * direction;
    camera_pos += move_speed * (float)(current_time - camera_speed_tic[0]);
    camera_speed_tic[0] = current_time;
  }
  if (camera_speed_mask[1] != 0) {
    move_speed = (float)camera_speed_mask[1] * move_sensitivity_ * right;
    camera_pos += move_speed * (float)(current_time - camera_speed_tic[1]);
    camera_speed_tic[1] = current_time;
  }
  return {camera_pos, direction, right};
}

} // namespace glpp::ui