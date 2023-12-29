// Fragment Shader
#version 330 core

layout (location = 0) out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    FragColor = vec4(FragPos.y / 5.0);
}