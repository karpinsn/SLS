varying vec4 projectorPos;

uniform float fringeFrequency;

uniform float thresholdMap[64];

void main()
{
	//	Constants for encoding
	float pi = 3.14159265;
	//float fringeFrequency = 4.0;
	float stepHeight = 1.0 / (2.0 * fringeFrequency);
	float stepWidth = 2.0 / (2.0 * fringeFrequency);
	float angularFrequency = 2.0 * pi * fringeFrequency;
	float stairAngularFrequency = angularFrequency * (4.0 + .5);
	
	float b = (floor(projectorPos.x * fringeFrequency) * stepHeight) + .5;
	
	// floor(projectorPos.x * fringeFrequency) * stepHeight) + .5
	
    vec4 color   = vec4((1.0 - sin(angularFrequency * projectorPos.x)) * .5, 
						(1.0 - cos(angularFrequency * projectorPos.x)) * .5, 
						cos(stairAngularFrequency * (projectorPos.x - (b * (1.0 / stepHeight) * stepWidth)) + pi) * (stepHeight / 3.0) + (stepHeight / 2.0) + b, 
						//b,
						1.0);
    
    float width = 512.0;
    float height = 512.0;

    float x = projectorPos.x * width;
    float y = projectorPos.y * height;

    int tx = int(mod(x, 8.0));
    int ty = int(mod(y, 8.0));

    int i = (8 * tx) + ty;
    color.x = color.x + thresholdMap[i] >= .5 ? 1.0 : 0.0;
    color.y = color.y + thresholdMap[i] >= .5 ? 1.0 : 0.0; 

    gl_FragColor = color;
}

