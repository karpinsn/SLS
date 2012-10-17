#version 130	// This is needed for the round operator

precision highp float;

uniform sampler2D fringeImage1;
uniform sampler2D fringeImage2;

in vec2 fragTexCoord;
out vec4 wrappedPhase;

void main(void)
{
	float pi = 3.14159;
	float twoPi = 2.0 * pi;
	
	float pitch1 = 30;
    float pitch2 = 768;

	vec3 fringe1 = texture(fringeImage1, fragTexCoord).rgb;
	vec3 fringe2 = texture(fringeImage2, fragTexCoord).rgb;

    float sine1 = sqrt(3.0) * (fringe1.r - fringe1.b);
    float cosine1 = 2.0 * fringe1.g - fringe1.r - fringe1.b;	

	float phaseShift = pi / 3.0;	
    float sine2Unshift = sqrt(3.0) * (fringe2.r - fringe2.b);
    float cosine2Unshift = 2.0 * fringe2.g - fringe2.r - fringe2.b;

    float phi2 = atan(sine2Unshift, cosine2Unshift);
	
	float sine2 = sin(phi2 - phaseShift);
	float cosine2 = cos(phi2 - phaseShift);
	
    float gamma = sqrt(pow((2 * fringe1.g - fringe1.r - fringe1.b), 2) + 3 * pow((fringe1.r - fringe1.b), 2)) / (fringe1.r + fringe1.g + fringe1.b);
	float phi1 = atan(sine1, cosine1);
	
    wrappedPhase = vec4(phi1, gamma, sine2, cosine2);		
}

