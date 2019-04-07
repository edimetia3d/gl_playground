//
// Created by edimetia3d on 2019-03-31.
//

#include "include/GLFW_FPS_Camera.h"

void GLFW_FPS_Camera::UpdateByKey(GLFWwindow *window, double last_frame_duration) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float camera_distance = move_speed_ * last_frame_duration;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraAt += cameraDirection * camera_distance;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraAt -= cameraDirection * camera_distance;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraAt -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * camera_distance;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraAt += glm::normalize(glm::cross(cameraDirection, cameraUp)) * camera_distance;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void GLFW_FPS_Camera::UpdateByMouse(double xpos, double ypos) {
    if (first_update_mouse_) {
        lastX = xpos;
        lastY = ypos;
        first_update_mouse_ = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDirection = glm::normalize(front);
}


glTransform_t GLFW_FPS_Camera::GetTransfrom(bool use_perspective) {
    glTransform_t ret;
    ret.LookAt(cameraAt.x,
               cameraAt.y,
               cameraAt.z,
               cameraAt.x + cameraDirection.x,
               cameraAt.y + cameraDirection.y,
               cameraAt.z + cameraDirection.z,
               cameraUp.x,
               cameraUp.y,
               cameraUp.z);
    if (use_perspective) {
        ret.Perspective(fov_, width_ / height_, near_dist_, far_dist_);
    } else {
        ret.Ortho(width_, height_, near_dist_, far_dist_);
    }
    return ret;

}
