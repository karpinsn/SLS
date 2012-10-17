#version 130

uniform sampler2D phaseMap;
uniform float width;

in vec2 fragTexCoord;

out vec4 depthMap;

void main()
{	
        float pi = 3.14159265;                  // Mmmmmm PI
        float theta = pi / 6.0;                 // Angle between camera and projector. 30 degrees
        float W = width;			// Width of the image
        float fringeFrequency = 6.0;            // Frequency of the fringe in Hz
        float P = W / (2.0 * fringeFrequency);  // Pixels per period of the projector

	vec4 vertPosition = vec4(fragTexCoord.s - .5, fragTexCoord.t - .5, 0.0, 0.0);
	float phaseR = (vertPosition.x * W) * ((2.0*pi*cos(theta))/P);
	float phaseA = texture(phaseMap, fragTexCoord).x;
			
	if(0.0 == phaseA)
	{
		depthMap = vec4(0.0);
	}
	else
	{
		depthMap = vec4((phaseA - phaseR) * P / (W * sin(theta)*2.0*pi));
	}
}
