#version 120

uniform sampler2D depthMap;

in vec3 vert;

out vec3 fragVert;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	float depth = texture2D(depthMap, gl_MultiTexCoord0.st).x;
	
	vec4 newVertexPosition = vert;	
	newVertexPosition.z = depth;

	v = vec3(gl_ModelViewMatrix * newVertexPosition);
	gl_Position = gl_ModelViewProjectionMatrix * newVertexPosition;
} 
