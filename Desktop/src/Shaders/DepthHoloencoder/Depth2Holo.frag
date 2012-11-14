#version 130

uniform sampler2D depthMap;
uniform float fringeFrequency;

in vec2 fragTexCoord;
out vec4 holoimage;

void main()
{
	//	Constants for encoding
	float pi = 3.14159265;
	float stepHeight = 1.0 / fringeFrequency;
	float stepWidth = 1.0 / fringeFrequency;
	float angularFrequency = 2.0 * pi * fringeFrequency;
	float stairAngularFrequency = angularFrequency * (4.0 + .5);
	float depth = texture(depthMap, fragTexCoord).x;
	
	float k = (floor(depth * fringeFrequency) * stepHeight);
	
	holoimage = vec4((1.0 - sin(angularFrequency * depth)) * .5, 
						(1.0 - cos(angularFrequency * depth)) * .5, 
						cos(stairAngularFrequency * (depth - (k * (1.0 / stepHeight) * stepWidth)) + pi) * (stepHeight / 3.0) + (stepHeight / 2.0) + k, 
						1.0);
}

