#version 330 core
layout (location = 0) in vec3 in_pos;
layout (location = 1) in vec2 in_texture_pos;
out vec2 texture_pos;
uniform mat4 transform;
void main()
{
    gl_Position = transform * vec4(in_pos.x, in_pos.y, in_pos.z, 1.0);
    texture_pos = vec2(in_texture_pos.x, 1.0f-in_texture_pos.y);
}