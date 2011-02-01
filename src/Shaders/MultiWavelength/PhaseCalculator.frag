uniform sampler2D fringeImage1;
uniform sampler2D fringeImage2;
uniform sampler2D fringeImage3;

void main(void)
{
	float pi = 3.14159;

	vec3 fringe1 = texture2D(fringeImage1, gl_TexCoord[0].st).rgb;
	vec3 fringe2 = texture2D(fringeImage2, gl_TexCoord[0].st).rgb;
	vec3 fringe3 = texture2D(fringeImage3, gl_TexCoord[0].st).rgb;

	float phi1 = atan(sqrt(3.0) * (fringe1.r - fringe1.b), 2.0 * fringe1.g - fringe1.r - fringe1.b);
	float phi2 = atan(sqrt(3.0) * (fringe2.r - fringe2.b), 2.0 * fringe2.g - fringe2.r - fringe2.b);
	float phi3 = atan(sqrt(3.0) * (fringe3.r - fringe3.b), 2.0 * fringe3.g - fringe3.r - fringe3.b);

	float phi12 = fmod(phi1 - phi2, 2.0 * pi);
	float phi23 = fmod(phi2 - phi3, 2.0 * pi);
	float phi123 = fmod(phi12 - phi23, 2.0 * pi);
	float k = fround((8 * phi123 - phi1) / 2.0 * pi);

	float phase = k * 2 * pi + phi1;

	gl_FragData[0].x = phase;
}

