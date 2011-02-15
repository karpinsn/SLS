#version 330	// This is needed for the round operator

//precision highp float;

uniform sampler2D fringeImage1;
uniform sampler2D fringeImage2;
uniform sampler2D fringeImage3;

in vec2 fragTexCoord;
out vec4 phase;

void main(void)
{
	float pi = 3.14159;

	vec4 fringe1 = texture2D(fringeImage1, fragTexCoord);
	vec4 fringe2 = texture2D(fringeImage2, fragTexCoord);
	vec4 fringe3 = texture2D(fringeImage3, fragTexCoord);

	float phi1 = atan(sqrt(3.0) * (fringe1.r - fringe1.b), 2.0 * fringe1.g - fringe1.r - fringe1.b);
	float phi2 = atan(sqrt(3.0) * (fringe2.r - fringe2.b), 2.0 * fringe2.g - fringe2.r - fringe2.b);
	float phi3 = atan(sqrt(3.0) * (fringe3.r - fringe3.b), 2.0 * fringe3.g - fringe3.r - fringe3.b);

	float phi12 = mod(phi1 - phi2, 2.0 * pi);
	float phi23 = mod(phi2 - phi3, 2.0 * pi);
	float phi123 = mod(phi12 - phi23, 2.0 * pi);
	float k = round((8.0 * phi123 - phi1) / 2.0 * pi);

	//phase = fringe3;
	phase = vec4(phi123);
	//	phase = vec4(fragTexCoord.x, fragTexCoord.y, .6, 1.0);
	//phase = vec4(k * 2.0 * pi + phi1);
}

