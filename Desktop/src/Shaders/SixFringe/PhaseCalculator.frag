#version 130	// This is needed for the round operator

precision highp float;

uniform sampler2D fringeImage1;
uniform sampler2D fringeImage2;

uniform float gammaCutoff;

in vec2 fragTexCoord;
out vec4 phase;

void main(void)
{
	float pi = 3.14159;
	float twoPi = 2.0 * pi;
	
	float pitch1 = 30;
    float pitch2 = 768;

	vec3 fringe1 = texture2D(fringeImage1, fragTexCoord).rgb;
	vec3 fringe2 = texture2D(fringeImage2, fragTexCoord).rgb;

    float sine1 = sqrt(3.0) * (fringe1.r - fringe1.b);
    float cosine1 = 2.0 * fringe1.g - fringe1.r - fringe1.b;	

	float phaseShift = pi / 3.0;	
    float sine2 = sqrt(3.0) * (fringe2.r - fringe2.b);
    float cosine2 = 2.0 * fringe2.g - fringe2.r - fringe2.b;

    float phase1 = atan(sine1, cosine1);
    float phi2 = atan(sine2, cosine2);
	float cosine22 = cos(phi2 - phaseShift);
	float sine22 = sin(phi2 - phaseShift);
	float phase2 = atan(sine22, cosine22);

	float magicNumber = .36;
    float k = round((phase2 * pitch2 / pitch1 - phase1) / twoPi - magicNumber);

    float gamma = sqrt(pow((2 * fringe1.g - fringe1.r - fringe1.b), 2) + 3 * pow((fringe1.r - fringe1.b), 2)) / (fringe1.r + fringe1.g + fringe1.b);
    		
    if(gamma >= gammaCutoff && !isnan(phase1))
    {
	    phase = vec4(phase1 + twoPi * k);
	}
    else
    {
        phase = vec4(0.0);
    }
}

