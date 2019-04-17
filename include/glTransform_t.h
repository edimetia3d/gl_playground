//
// Created by edimetia3d on 2019-03-30.
//

#ifndef GL_PLAYGROUND_GLMATRIX_T_H
#define GL_PLAYGROUND_GLMATRIX_T_H

#include <memory>

namespace MatrixPrivate {
    struct glMatrix_Impl;
}
/**
 * A col majored 4*4 matrix
 */
class glTransform_t {
public:
    glTransform_t();

    ~glTransform_t();

    glTransform_t(const glTransform_t &rval);

    glTransform_t(glTransform_t &&rval) noexcept;

    glTransform_t &operator=(const glTransform_t &rval);

    glTransform_t &operator=(glTransform_t &&rval) noexcept;

    void Reset();

    void RightMul(const glTransform_t &rhand);

    void Translate(float dx, float dy, float dz);

    void Rotate(float degree, float direction_x, float direction_y, float direction_z);

    void Scale(float ratio_x, float ratio_y, float ratio_z);

    void LookAt(float at_pos_x, float at_pos_y, float at_pos_z,
                float to_pos_x, float to_pos_y, float to_pos_z,
                float up_dir_x, float up_dir_y, float up_dir_z);

    void Perspective(float up_down_fov_degree,
                     float width_height_ratio,
                     float near_plane_dist,
                     float far_plane_dist);

    void Ortho(float window_width, float window_height, float near_plane_dist, float far_plane_dist);

    float *Data();

private:
    std::unique_ptr<MatrixPrivate::glMatrix_Impl> data;
};

#endif //GL_PLAYGROUND_GLMATRIX_T_H
