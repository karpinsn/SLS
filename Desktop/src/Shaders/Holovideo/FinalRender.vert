#version 130

uniform sampler2D depthMap;
uniform float width;

in vec4 vert;
in vec2 vertTexCoord;

varying vec3 v;

void main()
{	
	gl_TexCoord[0].st = vertTexCoord;

	vec4 newVertexPosition = vert;		
	newVertexPosition.z = texture(depthMap, vertTexCoord).x;

	v = vec3(gl_ModelViewMatrix * newVertexPosition);
	gl_Position = gl_ModelViewProjectionMatrix * newVertexPosition;
} 
