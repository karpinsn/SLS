varying float index;

void main()
{
	//	Constants for encoding
	float pi = 3.14159265;
	float frequency = 8.0;			 				//	Frequency of the fringe
	float angularFrequency = 2.0 * pi * frequency;	//	Angular frequency of the fringe
	float stepHeight = 1.0 / (2.0 * frequency);				//	Make the step height as large as possible
	float P = 32.0;
	float T = P/4.5;
	
	gl_FragColor = vec4((1.0 - sin(angularFrequency * index)) * .5, 
						(1.0 - cos(angularFrequency * index)) * .5, 
						(floor(index * frequency) * stepHeight + .5), 1.0);
}
