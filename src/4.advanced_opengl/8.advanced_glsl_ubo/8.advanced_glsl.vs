#version 420 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform Matrices1
{
    mat4 projection;
};

layout (std140, binding = 1) uniform Matrices2
{
    mat4 view;
};

uniform mat4 model;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}  