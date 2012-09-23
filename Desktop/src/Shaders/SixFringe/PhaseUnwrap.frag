#version 130	// This is needed for the round operator

precision highp float;

uniform sampler2D unfilteredWrappedPhase;
uniform sampler2D filteredWrappedPhase;

//uniform float gammaCutoff;

in vec2 fragTexCoord;
out vec4 phase;

void main(void)
{
	float pi = 3.14159;
	float twoPi = 2.0 * pi;
	
	float pitch1 = 30;
    float pitch2 = 768;

	vec2 cs1 = texture2D(unfilteredWrappedPhase, fragTexCoord).rg;
	vec2 cs2 = texture2D(filteredWrappedPhase, fragTexCoord).ba;

	float phase1 = atan(cs1.x, cs1.y);
	float phase2 = atan(cs2.x, cs2.y);

	float magicNumber = .36;
    float k = round((phase2 * pitch2 / pitch1 - phase1) / twoPi - magicNumber);

	phase = vec4(phase1 + twoPi * k);
	
	/*
    float gamma = sqrt(pow((2 * fringe1.g - fringe1.r - fringe1.b), 2) + 3 * pow((fringe1.r - fringe1.b), 2)) / (fringe1.r + fringe1.g + fringe1.b);
    		
    if(gamma >= gammaCutoff && !isnan(phase1))
    {
	    phase = vec4(phase1 + twoPi * k);
	}
    else
    {
        phase = vec4(0.0);
    }*/
}

