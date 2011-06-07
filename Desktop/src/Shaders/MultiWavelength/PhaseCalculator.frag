#version 330	// This is needed for the round operator

precision highp float;

uniform sampler2D fringeImage1;
uniform sampler2D fringeImage2;
uniform sampler2D fringeImage3;

in vec2 fragTexCoord;
out vec4 phase;

void main(void)
{
	float pi = 3.14159;

	vec3 fringe1 = texture2D(fringeImage1, fragTexCoord).rgb;
	vec3 fringe2 = texture2D(fringeImage2, fragTexCoord).rgb;
	vec3 fringe3 = texture2D(fringeImage3, fragTexCoord).rgb;

	float phi1 = atan(sqrt(3.0) * (fringe1.r - fringe1.b), 2.0 * fringe1.g - fringe1.r - fringe1.b);
	float phi2 = atan(sqrt(3.0) * (fringe2.r - fringe2.b), 2.0 * fringe2.g - fringe2.r - fringe2.b);
	float phi3 = atan(sqrt(3.0) * (fringe3.r - fringe3.b), 2.0 * fringe3.g - fringe3.r - fringe3.b);

	float phi12 = mod(phi1 - phi2, 2.0 * pi);
	float phi13 = mod(phi1 - phi3, 2.0 * pi);
	float phi123 = mod(phi12 - phi13, 2.0 * pi);
	
	float P1=60.0;
	float P2=96.0;
	float P3=90.0;
	float P12=(P1*P2)/(P2-P1);
	float P13=(P1*P3)/(P3-P1);
	float P123=(P13*P12)/(P13-P12);
	float k=floor((phi123*(P123/P1)-phi1)/(2.0*pi));

	float fringeMagnitude = length(fringe1) + length(fringe2) + length(fringe3);

	if(fringeMagnitude >= .015)
	{	
		phase = vec4(k * 2.0 * pi + phi1);
		//phase = vec4(phi1);
	}
	else
	{
		phase = vec4(0.0);
	}	
}

