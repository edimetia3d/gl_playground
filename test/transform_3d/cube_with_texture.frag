#version 330 core
in vec2 texture_pos;
uniform sampler2D tile_texture;
out vec4 FragColor;
void main()
{
    FragColor = texture(tile_texture, texture_pos);
}