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
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS 64

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
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;
uniform int pointLightNum;

uniform sampler2D shadowMap;



float ShadowCalculation(vec4 fragPosLightSpace);
float BlinnSpec(vec3 lightDir, vec3 normal, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main () 
{
	if(!lightOn)
	{
		fFragClr = texture(material.diffuse, uvCoord) * vec4(vec3(1.0), transparency)  * vec4(uColor, 1.0);
		return;
	}

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	for(int i = 0; i < pointLightNum; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	//result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    
	
    fFragClr = vec4(result, 1.0) * vec4(vec3(1.0), transparency) * vec4(uColor, 1.0) ;
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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
	float spec = BlinnSpec(lightDir, normal, viewDir);


    vec3 ambient = light.ambient * texture(material.diffuse, uvCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, uvCoord).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, uvCoord).rgb;

    if (shadowRender)
    {
        float shadow = ShadowCalculation(FragPosLightSpace);
        return (ambient + (1.0 - shadow) * (diffuse + specular));
    }
    else
    {
        return (ambient + diffuse + specular);
    }

    
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
	float spec = BlinnSpec(lightDir, normal, viewDir);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    vec3 ambient  = light.ambient  * texture(material.diffuse, uvCoord).rgb;
    vec3 diffuse  = light.diffuse  * diff * texture(material.diffuse, uvCoord).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, uvCoord).rgb;
    ambient  *= attenuation; 
	diffuse  *= attenuation;
    specular *= attenuation;

    if (shadowRender)
    {
        float shadow = ShadowCalculation(FragPosLightSpace);
        return (ambient + (1.0 - shadow) * (diffuse + specular));
    }
    else
    {
        return (ambient + diffuse + specular);
    }
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
	float spec = BlinnSpec(lightDir, normal, viewDir);
    

	float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * texture(material.diffuse, uvCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, uvCoord).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, uvCoord).rgb;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    if (shadowRender)
    {
        float shadow = ShadowCalculation(FragPosLightSpace);
        return (ambient + (1.0 - shadow) * (diffuse + specular));
    }
    else
    {
        return (ambient + diffuse + specular);
    }
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
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}
