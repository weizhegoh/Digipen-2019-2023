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
uniform vec3 uColor;


uniform sampler2D screenTexture;
in vec2 TexCoords;

void main () {
  fFragClr =  texture(screenTexture, TexCoords) * vec4(uColor, 1.0);
}





