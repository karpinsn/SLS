#version 120

uniform sampler2D phaseMap;
uniform float width;

varying vec3 v;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;

        float pi = 3.14159265;                  // Mmmmmm PI
        float theta = pi / 6.0;                 // Angle between camera and projector. 30 degrees
        float W = width;			// Width of the image
        float fringeFrequency = 4.0;            // Frequency of the fringe in Hz
        float P = W / (2.0 * fringeFrequency);  // Pixels per period of the projector
	
	float phaseR = (gl_Vertex.x * W) * ((2.0*pi*cos(theta))/P);
	float phaseA = texture2D(phaseMap, gl_MultiTexCoord0.st).x;
	vec4 newVertexPosition = gl_Vertex;
		
	newVertexPosition.z = (phaseA - phaseR) * P / (W * sin(theta)*2.0*pi);

	v = vec3(gl_ModelViewMatrix * newVertexPosition);
	gl_Position = gl_ModelViewProjectionMatrix * newVertexPosition;
} 
