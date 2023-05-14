#version 450 core
layout (location=0) out vec4 fFragClr;
in vec3 uColor;


void main()
{
    fFragClr = vec4(uColor , 1.0);   
}