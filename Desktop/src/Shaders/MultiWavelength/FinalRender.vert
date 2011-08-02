#version 120

uniform sampler2D depthMap;

in vec4 vert;
in vec2 vertTexCoord;

varying vec4 fragVert;
varying vec2 fragTexCoord;

void main()
{	
	fragTexCoord = vertTexCoord;
		
	float depth = texture2D(depthMap, vertTexCoord).x;
			
	vec4 newVertexPosition = vert;	
	newVertexPosition.z = depth;
	
	fragVert = gl_ModelViewMatrix * newVertexPosition;
	gl_Position = gl_ModelViewProjectionMatrix * newVertexPosition;
} 
