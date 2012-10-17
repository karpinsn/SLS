#version 130

uniform sampler2D depthMap;
uniform float width;

in vec4 vert;
in vec2 vertTexCoord;

out vec3 fragVert;

void main()
{	
	gl_TexCoord[0].st = vertTexCoord;

	vec4 newVertexPosition = vert;		
	newVertexPosition.z = texture(depthMap, vertTexCoord).x;

	fragVert = vec3(gl_ModelViewMatrix * newVertexPosition);
	gl_Position = gl_ModelViewProjectionMatrix * newVertexPosition;
} 
