#version 330

uniform sampler2D depthMap;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPosition;

in vec4 vert;
in vec2 vertTexCoord;

smooth out vec2 fragTexCoord;
smooth out vec3 lightDirection;

void main()
{	
	fragTexCoord = vertTexCoord.xy;
	lightDirection = lightPosition;
		
	float depth = texture2D(depthMap, vertTexCoord).x;
			
	vec4 newVertexPosition = vert;	
	newVertexPosition.z = depth;
		
	gl_Position = projectionMatrix * modelViewMatrix * newVertexPosition;
} 
