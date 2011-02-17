#version 120

uniform sampler2D phaseMap;
uniform sampler2D referencePhaseMap;

varying vec3 v;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	float phaseR = texture2D(referencePhaseMap, gl_MultiTexCoord0.st).x;
	float phaseA = texture2D(phaseMap, gl_MultiTexCoord0.st).x;
	vec4 newVertexPosition = gl_Vertex;
		
	newVertexPosition.z = (phaseA - phaseR);

	v = vec3(gl_ModelViewMatrix * newVertexPosition);
	gl_Position = gl_ModelViewProjectionMatrix * newVertexPosition;
} 
