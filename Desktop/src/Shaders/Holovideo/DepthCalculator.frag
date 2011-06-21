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
	
	float phaseR = (gl_Vertex.x * W) * ((2.0*pi*cos(theta))/P);
	float phaseA = texture2D(phaseMap, fragTexCoord).x;
	vec4 vertexPosition = vec4(fragTexCoord.s - .5, fragTexCoord.t - .5, 0.0, 0.0);
		
	vertexPosition.z = (phaseA - phaseR) * P / (W * sin(theta)*2.0*pi);

	depthMap = vertexPosition
	gl_Position = gl_ModelViewProjectionMatrix * newVertexPosition;
}
