#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;
layout (location = 7) in vec3 aColor;

out vec2 TexCoords;
out vec3 Color;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aTexCoords;
    Color = aColor;
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0f); 
}