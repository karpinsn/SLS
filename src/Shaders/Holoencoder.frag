varying vec4 projectorPos;

void main()
{
	//	Constants for encoding
	float pi = 3.14159265;
	float stepHeight = .2;
	float P = 256.0;
	float T = P/4.5;
	
	gl_FragColor = vec4((1.0 - sin(pi * 2.0 * projectorPos.x)) * .5, (1.0 - cos(pi * 2.0 * projectorPos.x)) * .5, ((floor(projectorPos.x) * cos(2.0*pi*projectorPos.x/T)) * stepHeight) + stepHeight, 1.0);
}

