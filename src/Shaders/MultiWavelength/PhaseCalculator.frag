uniform sampler2D fringe1;
uniform sampler2D fringe2;
uniform sampler2D fringe3;

void main(void)
{
	vec3 phi1 = texture2D(fringe1, gl_TexCoord[0].st).rgb;
	vec3 phi2 = texture2D(fringe2, gl_TexCoord[0].st).rgb;
	vec3 phi3 = texture2D(fringe3, gl_TexCoord[0].st).rgb;

	vec4 holoPhase = texture2D(holoImage, gl_TexCoord[0].st);

	float fringeFrequency = 4.0;							// Frequency of the fringe in Hz
	float pi = 3.14159265; 									// Mmmmmm PI
	float stepHeight = 1.0 / (2.0 * fringeFrequency) -.001;	// .001 is just a buffer so we dont get rounding errors

	float I1 = holoPhase.x * 255.0;
	float I2 = holoPhase.y * 255.0;
	float I3 = floor(holoPhase.z * 1.0 / stepHeight);

	float phaseA = atan((I1 - 127.5), (I2 - 127.5)) + (2.0 * pi * I3);

	gl_FragData[0].x = phaseA;
}

