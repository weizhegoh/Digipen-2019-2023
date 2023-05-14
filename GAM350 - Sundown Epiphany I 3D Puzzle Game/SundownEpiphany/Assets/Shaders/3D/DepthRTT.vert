#version 450 core

layout (location = 0) in vec3 vVertexPosition;
layout (location = 1) in vec2 uv;

out vec2 TexCoords;

void main()
{
    TexCoords = uv;
    gl_Position = vec4(vVertexPosition, 1.0);
}