/*****************************************************************
*\file		 Animation_vertexShader.vert
*\brief		 Vertex shader for animation.

*\author(s)   Lee Jun Jie	19 lines x 100% Code contribution
*\author(s)   Lee Liang Ping 3 lines x 100% Code contribution
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
uniform int uFrameX;
uniform int uFrameY;
uniform int uFrameIndex;
//uniform int uScreenRender;

void main() {

   gl_Position = uProjection * uView * uModel * vec4(vVertexPosition, 1.0);

   	/*if(screenRender == 0)
		gl_Position = uProjection * uView * uModel * vec4(vVertexPosition, 1.0);
	else if(screenRender == 1)
		gl_Position = uProjection * uModel * vec4(vVertexPosition, 1.0);*/


   //vClrCoord = vVertexClrCoord;
   vClrCoord = vec3(uModel * vec4(vVertexPosition, 1.0));

   float unitWidth = 1.0 / uFrameX;
   float unitHeight = 1.0 / uFrameY;

   int indexX = uFrameIndex % uFrameX;
   int indexY = uFrameIndex / uFrameX;

   vTexCoord.x = (vVertexTexture.x * unitWidth + unitWidth * indexX);
   vTexCoord.y = (vVertexTexture.y * unitHeight + unitHeight * (uFrameY - 1 - indexY));
}