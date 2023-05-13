/*****************************************************************
*\file		 TvertexShader.vert
*\brief		 Vertex shader for texture.

*\author(s)   Lee Liang Ping	11 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#version 450 core

layout (location=0) in vec3 vVertexPosition;
layout (location=1) in vec3 vVertexClrCoord;
layout (location=2) in vec2 vVertexTexture;

layout (location=0) out vec3 vClrCoord;
layout (location=1) out vec2 vTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
void main() {

	gl_Position = uProjection * uView * uModel * vec4(vVertexPosition, 1.0);
	
	vClrCoord = vec3(uModel * vec4(vVertexPosition, 1.0));
	//vClrCoord = vVertexClrCoord;
	vTexCoord = vVertexTexture;
}