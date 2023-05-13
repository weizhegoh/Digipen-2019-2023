/*****************************************************************
*\file         vertexShader.vert
*\brief         Vertex shader for shapes and colours.

*\author(s)   Lee Liang Ping    5 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#version 450 core

struct PointLight 
{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};


layout (location = 0) in vec3 vVertexPosition;
layout (location = 1) in vec3 vVertexNormal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tan;
layout (location = 4) in vec3 bit;
layout (location = 5) in ivec2 jointID;
layout (location = 6) in vec2 weights;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 lightSpaceMatrix;

#define NR_POINT_LIGHTS 24
uniform int pointLightNum;
uniform vec3 viewPos;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform bool parallaxMappingOn;

out vec3 FragPos;
out vec3 Normal;
out vec2 uvCoord;
out vec4 FragPosLightSpace;


out vec3 lightDir[NR_POINT_LIGHTS];
out vec3 viewDir;




void main()
{
    gl_Position = uProjection * uView * uModel * vec4(vVertexPosition, 1.0);
	FragPos = vec3(uModel * vec4(vVertexPosition, 1.0));
	Normal = normalize(mat3(uModel) * vVertexNormal);
	FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
	uvCoord = uv;
	
	viewDir = normalize(viewPos - FragPos);
	for (int i = 0; i < pointLightNum; ++i)    
		lightDir[i] = pointLights[i].position - FragPos; 

	
	if(parallaxMappingOn)
	{
		vec3 tangentVF   = normalize(mat3(uModel) * tan);                                        
		vec3 bitangentVF = normalize(mat3(uModel) * bit);
		mat3 Transpose = transpose(mat3(tangentVF, bitangentVF, Normal));
		viewDir = Transpose * viewDir;                                                                                    
		for (int i = 0; i < pointLightNum; ++i)                                             
			lightDir[i] = Transpose * lightDir[i];
	}
}