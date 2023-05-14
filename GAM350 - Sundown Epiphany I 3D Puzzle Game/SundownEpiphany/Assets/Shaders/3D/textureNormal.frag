/*****************************************************************
*\file		 fragmentShader.frag
*\brief		 Fragment shader for shapes and colours.

*\author(s)   Lee Liang Ping	3 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#version 450 core
layout (location=0) out vec4 fFragClr;
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform float transparency; 
uniform Material material;
uniform vec3 uColor; 
in vec2 uvCoord;
void main () 
{
	fFragClr = texture(material.diffuse, uvCoord) * vec4(vec3(1.0), transparency)  * vec4(uColor, 1.0);
}
