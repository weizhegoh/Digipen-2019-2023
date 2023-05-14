/*****************************************************************
*\file		 fragmentShader.frag
*\brief		 Fragment shader for shapes and colours.

*\author(s)   Lee Liang Ping	3 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#version 450 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	//sampler2D normalTex;
	//sampler2D bumpTex;  
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

#define NR_DIR_LIGHTS 16
#define NR_POINT_LIGHTS 24

layout (location=0) out vec4 fFragClr;

uniform vec3 uColor; 
uniform float transparency; 

in vec3 FragPos;
in vec3 Normal; 
in vec2 uvCoord;
in vec4 FragPosLightSpace;

uniform bool lightOn;
uniform bool blinn;
uniform bool shadowRender;

uniform vec3 viewPos;
uniform DirLight dirLights[NR_DIR_LIGHTS];
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform int pointLightNum;
uniform int dirLightNum;
uniform bool gamma;
uniform vec3 lightPos;
uniform sampler2D shadowMap;

uniform vec4 ambientColor;

float ShadowCalculation(vec4 fragPosLightSpace);
float BlinnSpec(vec3 lightDir, vec3 normal, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 specColour);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 diffColour, vec3 specColour); 

void main () 
{
	if(!lightOn)
	{
		fFragClr = texture(material.diffuse, uvCoord) * vec4(vec3(1.0), transparency)  * vec4(uColor, 1.0);
		if(gamma)
			fFragClr.rgb = pow(fFragClr.rgb, vec3(0.45454545));
		return;
	}

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos.xzy - FragPos);
	vec3 diffuseColour = texture(material.diffuse, uvCoord).rgb * uColor.rgb;
	vec3 specColour    = texture(material.specular, uvCoord).rgb * uColor.rgb;
	
	//vec3 ambient = ambientColor.rgb * ambientColor.a;
	
	vec3 result = vec3(0.0);
	for(int i = 0; i < pointLightNum; i++)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, diffuseColour, specColour);

    }

    for(int i = 0; i < dirLightNum; i++)
    {
        result += CalcDirLight(dirLights[i], norm, viewDir, specColour);
        if (shadowRender)
        {
            float shadow = ShadowCalculation(FragPosLightSpace);
            result *= (1.0 - shadow);
        }
    }
        
	
	
	//vec3 ambient = vec3(0.0);
	//vec3 intensity = min(vec3(1.0), ambient * diffuseColour + result);
	
	vec4 colour =vec4(diffuseColour, 1.0) * vec4(result, 1.0);
	if(gamma)
		colour.rgb = pow(colour.rgb, vec3(0.45454545));
    fFragClr = vec4(colour.rgb, 1.0) * vec4(vec3(1.0), transparency);

}

float BlinnSpec(vec3 lightDir, vec3 normal, vec3 viewDir)
{
	float spec = 0.f;
	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir, normal); 
		spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	}
	return spec;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 specColour)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
	float spec = BlinnSpec(lightDir, normal, viewDir);

	vec3 diffValue = light.diffuse.rgb  * light.diffuse.a;
	vec3 specValue = light.specular.rgb * light.specular.a;
    vec3 ambient = diffValue;
    vec3 diffuse = diffValue * diff;
    vec3 specular = specValue * specColour * spec;

    return (ambient + diffuse + specular);
    
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 lightDirection, vec3 specColour)
{
    vec3 lightDirr = normalize(lightDirection);
    float diff = max(dot(normal, lightDirr), 0.0);
	float spec = BlinnSpec(lightDirr, normal, viewDir);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	vec3 ambValue = light.ambient.rgb  * light.ambient.a;
	vec3 diffValue = light.diffuse.rgb  * light.diffuse.a;
	vec3 specValue = light.specular.rgb * light.specular.a;
	vec3 ambient   = ambValue   ;
    vec3 diffuse   = diffValue * diff;
    vec3 specular  = specValue * specColour * spec;
    ambient  *= attenuation; 
	diffuse  *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
} 

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    
    // check whether current frag pos is in shadow
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 18.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}
