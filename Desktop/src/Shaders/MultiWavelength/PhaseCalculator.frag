#version 130	// This is needed for the round operator

precision highp float;

uniform sampler2D fringeImage1;
uniform sampler2D fringeImage2;
uniform sampler2D fringeImage3;

uniform float gammaCutoff;

in vec2 fragTexCoord;
out vec4 phase;

void main(void)
{
	float pi = 3.14159;
	float twoPi = 2.0 * pi;

	vec3 fringe1 = texture2D(fringeImage1, fragTexCoord).rgb;
	vec3 fringe2 = texture2D(fringeImage2, fragTexCoord).rgb;
	vec3 fringe3 = texture2D(fringeImage3, fragTexCoord).rgb;

	float phi1 = atan(sqrt(3.0) * (fringe1.r - fringe1.b), 2.0 * fringe1.g - fringe1.r - fringe1.b) / twoPi + .5;
	float phi2 = atan(sqrt(3.0) * (fringe2.r - fringe2.b), 2.0 * fringe2.g - fringe2.r - fringe2.b) / twoPi + .5;
	float phi3 = atan(sqrt(3.0) * (fringe3.r - fringe3.b), 2.0 * fringe3.g - fringe3.r - fringe3.b) / twoPi + .5;

	float phi12 = mod(phi2 - phi1, 1.0);
	float phi13 = mod(phi3 - phi1, 1.0);
	float phi123 = mod(phi12 - phi13, 1.0);
	
	float P1=60.0;
	float P2=96.0;
	float P3=90.0;
	float P12=(P1*P2)/(P2-P1);
	float P13=(P1*P3)/(P3-P1);
	float P123=(P13*P12)/(P13-P12);

	float k13 = round(phi123 * (P123/P13) - phi13);
	float phase13 = k13 + phi13;

	float k3 = round(phase13 * (P13/P3) + phi3);
	float phase3 = phi3 - k3;
	
	float k = round(phi1 - (phase3 * (P3/P1)));	

	float gamma = sqrt(pow((2 * fringe1.g - fringe1.r - fringe1.b), 2) + 3 * pow((fringe1.r - fringe1.b), 2)) / (fringe1.r + fringe1.g + fringe1.b);

	if(gamma >= gammaCutoff)
	{	
		phase = vec4(phi1 - k);
	}
	else
	{
		phase = vec4(0.0);
	}	

	if(isnan(phi1))
	{
		phase = vec4(0.0);
	}
}

