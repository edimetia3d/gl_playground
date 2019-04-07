#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model_trans;
uniform mat4 look_trans;

void main()
{
    FragPos = vec3(model_trans * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model_trans))) * aNormal;
    TexCoords = vec2(aTexCoords.x,1.0f-aTexCoords.y);

    gl_Position = look_trans * vec4(FragPos, 1.0);
}