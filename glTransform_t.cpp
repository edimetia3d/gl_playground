//
// Created by edimetia3d on 2019-03-30.
//

#include "include/glTransform_t.h"

#include "dependences/glm/glm/glm.hpp"
#include "dependences/glm/glm/gtx/transform.hpp"
#include "dependences/glm/glm/gtc/type_ptr.hpp"

namespace MatrixPrivate {
    struct glMatrix_Impl {
        glm::mat4 mat = glm::mat4(1.0f);
    };
}
using namespace MatrixPrivate;

void glTransform_t::Translate(float dx, float dy, float dz) {
    data->mat = glm::translate(glm::vec3(dx, dy, dz)) * data->mat;
}

void glTransform_t::Rotate(float degree, float direction_x, float direction_y, float direction_z) {
    data->mat = glm::rotate(glm::radians(degree), glm::vec3(direction_x, direction_y, direction_z)) * data->mat;
}

void glTransform_t::Scale(float ratio_x, float ratio_y, float ratio_z) {
    data->mat = glm::scale(glm::vec3(ratio_x, ratio_y, ratio_z)) * data->mat;
}

float *glTransform_t::Data() {
    return glm::value_ptr(data->mat);
}

glTransform_t::glTransform_t() {
    data = std::make_unique<glMatrix_Impl>();
}

void glTransform_t::RightMul(const glTransform_t &rhand) {
    data->mat *= rhand.data->mat;
}

glTransform_t::~glTransform_t() {

}

glTransform_t::glTransform_t(glTransform_t &&rval) {
    data = std::move(rval.data);
}


void glTransform_t::Reset() {
    data->mat = glm::mat4(1.0f);
}

void glTransform_t::Perspective(float up_down_fov_degree, float width_height_ratio, float near_plane_dist,
                                float far_plane_dist) {
    data->mat = glm::perspective(glm::radians(up_down_fov_degree), width_height_ratio, near_plane_dist,
                                 far_plane_dist) * data->mat;
}

void
glTransform_t::Ortho(float window_width, float window_height, float near_plane_dist, float far_plane_dist) {

    data->mat = glm::ortho(-window_width / 2, window_width / 2, -window_height / 2, window_height, near_plane_dist,
                           far_plane_dist) * data->mat;

}

void
glTransform_t::LookAt(float at_pos_x, float at_pos_y, float at_pos_z, float to_pos_x, float to_pos_y, float to_pos_z,
                      float up_dir_x, float up_dir_y, float up_dir_z) {

    data->mat = glm::lookAt(glm::vec3(at_pos_x, at_pos_y, at_pos_z),
                            glm::vec3(to_pos_x, to_pos_y, to_pos_z),
                            glm::vec3(up_dir_x, up_dir_y, up_dir_z)) * data->mat;

}
