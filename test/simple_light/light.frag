#version 330 core
out vec4 FragColor;
uniform vec3 light_color;
void main()
{
    FragColor = vec4(light_color.x,light_color.y,light_color.z,1.0f);
}