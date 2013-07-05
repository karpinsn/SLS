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

	vec3 fringe1 = texture(fringeImage1, fragTexCoord).rgb;
	vec3 fringe2 = texture(fringeImage2, fragTexCoord).rgb;
	vec3 fringe3 = texture(fringeImage3, fragTexCoord).rgb;

	float phi1 = atan(sqrt(3.0) * (fringe1.r - fringe1.b), 2.0 * fringe1.g - fringe1.r - fringe1.b);
	float phi2 = atan(sqrt(3.0) * (fringe2.r - fringe2.b), 2.0 * fringe2.g - fringe2.r - fringe2.b);
	float phi3 = atan(sqrt(3.0) * (fringe3.r - fringe3.b), 2.0 * fringe3.g - fringe3.r - fringe3.b);

	float phi12 = -mod(phi2 - phi1, twoPi);
	float phi13 = -mod(phi3 - phi1, twoPi);

    //  Remove the phase jump
    phi12 = atan(sin(phi12),cos(phi12));
	
	float P1=90.0;
	float P2=102.0;
	float P3=135.0;
	float P12=(P1*P2)/abs(P1-P2);
	float P13=(P1*P3)/abs(P1-P3);

	float k13 = round((phi12 * (P12/P13) - phi13) / twoPi);
	float phase13 = phi13 + k13 * twoPi;

	float k = round((phase13 * (P13/P1) - phi1) / twoPi);	

	float gamma = sqrt(pow((2 * fringe1.g - fringe1.r - fringe1.b), 2) + 3 * pow((fringe1.r - fringe1.b), 2)) / (fringe1.r + fringe1.g + fringe1.b);

	if(gamma >= gammaCutoff && gamma <= 1.0 && !isnan(phi1))
	{	
		phase = vec4(phi1 + k * twoPi);
	}
	else
	{
        //  Sets the phase to inf
		phase = vec4(1.0/0.0);
	}	
}

