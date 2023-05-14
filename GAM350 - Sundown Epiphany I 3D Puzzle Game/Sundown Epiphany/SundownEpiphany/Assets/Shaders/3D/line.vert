#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 uColor;

uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
    gl_Position = uProjection * uView * vec4(position, 1.0f);

    uColor = color;
}