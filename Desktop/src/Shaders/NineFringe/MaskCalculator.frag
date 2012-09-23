#version 130	// This is needed for the round operator

precision highp float;

uniform sampler2D fringeImage1;
uniform sampler2D fringeImage2;
uniform sampler2D fringeImage3;

in vec2 fragTexCoord;
in vec4 phase;

out vec4 mask;

void main(void)
{
	vec2 offset[9];
  	offset[3] = vec2(-step_w, step_h); 	offset[2] = vec2(0.0, step_h); 	offset[1] = vec2(step_w, step_h);
  	offset[4] = vec2(-step_w, 0.0);   					offset[0] = vec2(step_w, 0.0);		offset[8] = vec2(step_w, 0.0);
  	offset[5] = vec2(-step_w, -step_h);  	offset[6] = vec2(0.0, -step_h);	offset[7] = vec2(step_w, -step_h); 

	float phaseValue = texture2D(phase, fragTexCoord).r;

	for(int i = 0; i < 8; ++i)
	{
		if(phaseValue)	
	}	
}

