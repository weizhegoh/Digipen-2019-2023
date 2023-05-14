#version 430 core

uniform sampler2D sky_bk;
uniform sampler2D sky_ft;
uniform sampler2D sky_lt;
uniform sampler2D sky_rt;
uniform sampler2D sky_up;
uniform sampler2D sky_dn;
uniform bool gamma;
in VS_OUT
{
    vec3 rasterColor;
	vec3 vertexNormal;
	vec2 uv;
} fs_in;

layout(location = 0) out vec4 color;
layout(location = 1) out vec4 emissive_mask;

void main()
{
	vec3 N = normalize(fs_in.vertexNormal);
	vec2 tmpuv = fs_in.uv;
	// tmpuv.y = 1.0f - tmpuv.y;
	
	float abx = abs(N.x);
	float aby = abs(N.y);
	float abz = abs(N.z);
	
	if (abx >= aby)
	{
		if (abx >= abz)
		{
			// x is the max
			if (N.x >= 0)
			{
				// use X texture
				color = texture(sky_lt, tmpuv);
			}
			else
			{
				// use -X texture
				color = texture(sky_rt, tmpuv);
			}
		}
		else
		{
			// z is the max
			if (N.z >= 0)
			{
				// use Z texture
				color = texture(sky_ft, tmpuv);
			}
			else
			{
			    // use -Z texture
				color = texture(sky_bk, tmpuv);
			}
		}
	}
	else
	{
		if (aby >= abz)
		{
			// y max
			if (N.y >= 0)
			{
				// use Y texture
				color = texture(sky_dn, tmpuv);
			}
			else
			{
				// use -Y texture
				color = texture(sky_up, tmpuv);
			}
		}
		else
		{
			// z max
			if (N.z >= 0)
			{
				// use Z texture
				color = texture(sky_ft, tmpuv);
			}
			else
			{
				// use -Z texture
				color = texture(sky_bk, tmpuv);
			}
		}
	}
	if(gamma)
		color.rgb = pow(color.rgb, vec3(0.45454545));
	emissive_mask = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
