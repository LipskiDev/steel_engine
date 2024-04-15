// Vertex Shader
#version 410 core

layout (location = 0) in vec3 aPos;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;


void main()
{   
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = model * vec4(FragPos, 1.0);
}