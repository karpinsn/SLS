#version 120

uniform sampler2D depthMap;

varying vec3 v;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	float depth = texture2D(depthMap, gl_MultiTexCoord0.st).x;
	
	vec4 newVertexPosition = gl_Vertex;	
	newVertexPosition.z = depth;

	v = vec3(gl_ModelViewMatrix * newVertexPosition);
	gl_Position = gl_ModelViewProjectionMatrix * newVertexPosition;
} 
