uniform sampler2D holoImage;

void main(void)
{
	vec4 holoPhase = texture2D(holoImage, gl_TexCoord[0].st);

	float P = 256.0;			// Pixels per period of the projector
	float W = 512.0;			// Width of the image
	float pixelWidth = 1.0 / W;	// Width of a pixel
	float pi = 3.14159265; 		// Mmmmmm PI
	float theta = pi / 6.0;		// Angle between camera and projector. 30 degrees

	float stairHeight = 51.0;		//	51

	float I1 = holoPhase.x * 255.0;
	float I2 = holoPhase.y * 255.0;
	float I3 = floor(holoPhase.z * 255.0 / stairHeight);

	float phaseA = atan((I1 - 127.5), (I2 - 127.5)) + (2.0 * pi * I3);

	gl_FragData[0].x = phaseA;
}
