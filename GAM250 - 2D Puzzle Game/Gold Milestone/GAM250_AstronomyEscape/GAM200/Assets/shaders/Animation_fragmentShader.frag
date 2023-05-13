/*****************************************************************
*\file		 Animation_fragmentShader.frag
*\brief		 Fragment shader for animation.

*\author(s)   Lee Liang Ping	30 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#version 450 core

struct PointLight {
	vec3 color;
	vec3 position;
	float ambient;
	float constant;
	float linear;
	float quadratic;
};
#define MAX_LIGHTS 128
uniform PointLight pointLights[MAX_LIGHTS];


layout(location=0) in vec3 vClrCoord;
layout (location=1) in vec2 vTexCoord;

layout (location=0) out vec4 fFragClr;

uniform vec4 uAmbientColor;
uniform sampler2D uTex2d;
uniform float transparency;

uniform vec3 viewPos;
uniform int activeLights;
uniform bool isLight;

vec4 CalcPointLight(PointLight light, vec3 clrcoord);

void main () {

	vec3 highlight = uAmbientColor.xyz * uAmbientColor.a;
	vec4 result = texture(uTex2d, vTexCoord) ;

	if (isLight) 
	{ 
		int minActiveLights = min(activeLights, MAX_LIGHTS);
		for (int i = 0; i < minActiveLights; ++i)
			result += CalcPointLight(pointLights[i], vClrCoord);
	}

	fFragClr = result * vec4(vec3(1.0), transparency) * vec4(highlight, 1.0);
}


vec4 CalcPointLight(PointLight light, vec3 clrcoord)
{
    float distance = length(light.position - clrcoord);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient = light.ambient * light.color * vec3(texture(uTex2d, vTexCoord));
    ambient *= attenuation;
    return vec4(ambient, 0.0);
}