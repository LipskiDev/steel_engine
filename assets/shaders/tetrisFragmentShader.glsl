// Fragment Shader
#version 410 core

layout (location = 0) out vec4 FragColor;

in vec3 FragPos;
in vec3 aColor;


void main()
{
    FragColor = vec4(aColor, 1.0);
}
