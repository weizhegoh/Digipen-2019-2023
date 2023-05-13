/*****************************************************************
*\file         vertexShader.vert
*\brief         Vertex shader for shapes and colours.

*\author(s)   Lee Liang Ping    5 lines x 100% Code contribution

Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*******************************************************************/
#version 450 core

layout (location = 0) in vec3 vVertexPosition;
layout (location = 1) in vec3 vVertexNormal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tan;
layout (location = 4) in vec3 bit;
layout (location = 5) in ivec4 jointID;
layout (location = 6) in vec4  weights;

uniform mat4 lightSpaceMatrix;
uniform mat4 uModel;
uniform bool isSkeletalMesh;

const int MAX_BONES = 60;
const int MAX_BONE_INFLUENCE = 10;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    
    if (isSkeletalMesh)
    {
        vec4 totalPosition = vec4(0.0f);
        for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
        {
            if(jointID[i] == -1) 
                continue;

            if(jointID[i] >=MAX_BONES) 
            {
                totalPosition = vec4(vVertexPosition,1.0f);
                break;
            }
            vec4 localPosition = finalBonesMatrices[jointID[i]] * vec4(vVertexPosition,1.0f);
            totalPosition += localPosition * weights[i];
            vec3 localNormal = mat3(finalBonesMatrices[jointID[i]]) * vVertexNormal;

        }

        gl_Position = lightSpaceMatrix * uModel * totalPosition;
    }
    else
    {
        gl_Position = lightSpaceMatrix * uModel * vec4(vVertexPosition,1.0f);
    }
 
}