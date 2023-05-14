#version 430 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float Brightness;
uniform float Contrast;
uniform vec4 AverageLuminance;

void main()
{ 
    vec4 texColour = texture(screenTexture, TexCoords);

	FragColor =  mix(texColour * Brightness, mix(AverageLuminance, texColour, Contrast), 0.5);
	
	//FragColor = vec4(((texColour.rgb - vec3(0.5)) * Contrast + vec3(0.5)) + vec3(Brightness), texColour.w);
	
	
}