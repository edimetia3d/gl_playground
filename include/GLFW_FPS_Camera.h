//
// Created by edimetia3d on 2019-03-31.
//

#ifndef GL_PLAYGROUND_EXGLFWCAMERA_H
#define GL_PLAYGROUND_EXGLFWCAMERA_H

#include "glTransform_t.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class GLFW_FPS_Camera {
public:
    void UpdateByKey(GLFWwindow *window, double last_frame_duration);

    void UpdateByMouse(double xpos, double ypos);

    glTransform_t GetTransfrom(bool use_perspective = true);

    glTransform_t GetViewOnly();

    glTransform_t GetProjectionOnly(bool use_perspective = true);

    void SetWidth(float width) {
        width_ = width;
    }

    void SetHeight(float height) {
        height_ = height;
    }

    void SetFOV(float fov) {
        fov_ = fov;
    }

    void SetNearDist(float near_dist) {
        near_dist_ = near_dist;
    }

    void SetFarDist(float far_dist) {
        far_dist_ = far_dist;
    }

    void SetCameraMoveSpeed(float speed) {
        move_speed_ = speed;
    }

    const glm::vec3 & GetCameraPos(){
        return cameraAt;
    }

private:
    bool first_update_mouse_ = true;


    float width_ = 800;
    float height_ = 600;
    float fov_ = 45;

    float near_dist_ = 0.1f;
    float far_dist_ = 100.0f;

    float move_speed_ = 2.0f;
    glm::vec3 cameraAt = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); // we are not flying a aircraft , so up is always up

    float lastX;
    float lastY;
    float sensitivity = 0.1f;
    float yaw = -90.0f;
    float pitch = 0.0f;
};


#endif //GL_PLAYGROUND_EXGLFWCAMERA_H
