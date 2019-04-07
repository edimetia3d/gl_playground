#version 330 core
layout (location = 0) in vec3 in_pos;
uniform mat4 model_trans;
uniform mat4 look_trans;
void main()
{
    gl_Position = look_trans * model_trans * vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);
}