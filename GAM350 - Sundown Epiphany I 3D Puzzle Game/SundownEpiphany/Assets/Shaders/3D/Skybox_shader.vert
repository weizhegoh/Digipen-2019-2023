#version 430 core

uniform mat4  MVP;
uniform mat4  InvModel;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

layout(location = 0) in vec3    vPosition;
layout(location = 1) in vec3    vertexNormal;
layout(location = 2) in vec2    vUV;

out VS_OUT
{
    vec3 rasterColor;
	vec3 vertexNormal;
	vec2 uv;
} vs_out;

void main()
{
	vs_out.vertexNormal = normalize(InvModel * vec4( vertexNormal, 0.0f )).xyz;
	vs_out.uv = vUV;
  gl_Position = MVP * vec4(vPosition, 1.0f);
}
