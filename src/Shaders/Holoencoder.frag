varying vec4 projectorPos;

void main()
{
	//	Constants for encoding
	float pi = 3.14159265;
	float fringeFrequency = 4.0;
	float stepHeight = 1.0 / (2.0 * fringeFrequency);
	float angularFrequency = 2.0 * pi * fringeFrequency;
	
	gl_FragColor = vec4((1.0 - sin(angularFrequency * projectorPos.x)) * .5, 
						(1.0 - cos(angularFrequency * projectorPos.x)) * .5, 
						(floor(projectorPos.x * fringeFrequency) * stepHeight) + .5, 
						1.0);
}

