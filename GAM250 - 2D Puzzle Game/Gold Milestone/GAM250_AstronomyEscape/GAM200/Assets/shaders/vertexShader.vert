/*****************************************************************
*\file		 vertexShader.vert
*\brief		 Vertex shader for shapes and colours.

*\author(s)   Lee Liang Ping	5 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/

#version 450 core
layout (location=0) in vec3 vVertexPosition;
//layout (location=1) in vec3 vVertexClrCoord;
//layout (location=2) in vec2 vVertexTexture;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
//uniform int uScreenRender;
void main() {

   	/*if(screenRender == 0)
		gl_Position = uProjection * uView * uModel * vec4(vVertexPosition, 1.0);
	else if(screenRender == 1)
		gl_Position = uProjection * uModel * vec4(vVertexPosition, 1.0);*/

   gl_Position = uProjection * uView * uModel * vec4(vVertexPosition, 1.0);
   //vClrCoord = vVertexClrCoord;
   //vClrCoord = vec3(1.0f,1.0f,0.0f);
}
