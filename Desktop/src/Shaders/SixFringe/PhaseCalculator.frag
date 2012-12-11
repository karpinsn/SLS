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
	float tolerance = .01;
	
	float pitch1 = 74;
    float pitch2 = 79;
    float pitch12 = (pitch1 * pitch2) / abs(pitch1 - pitch2);

	vec3 fringe1 = texture(fringeImage1, fragTexCoord).rgb;
	vec3 fringe2 = texture(fringeImage2, fragTexCoord).rgb;

	float phi1 = atan(sqrt(3.0) * (fringe1.r - fringe1.b), 2.0 * fringe1.g - fringe1.r - fringe1.b);	
	float phi2 = atan(sqrt(3.0) * (fringe2.r - fringe2.b), 2.0 * fringe2.g - fringe2.r - fringe2.b);

    float phi12 = mod(phi1 - phi2, 2.0 * pi);
    float k = round(((phi12 * (pitch12/pitch1) - phi1) - tolerance) / twoPi);
 
    float gamma = sqrt(pow((2 * fringe1.g - fringe1.r - fringe1.b), 2) + 3 * pow((fringe1.r - fringe1.b), 2)) / (fringe1.r + fringe1.g + fringe1.b);
     		
	if(gammaCutoff <= gamma)
	{
		//	Just setting phase to be an invalid number
		phase = vec4(vec3(phi1 + k * 2.0 * pi), 0.0);
	}
    else
    {
        phase = vec4(vec3(0.0/0.0), 0.0);
    }
}

