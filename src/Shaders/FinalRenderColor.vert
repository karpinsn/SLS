#version 330

precision highp float;

uniform sampler2D depthMap;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPosition;

in vec4 vert;
in vec2 vertTexCoord;

out vec3 lightDirection;
out vec3 eyeVector;
out vec2 fragTexCoord;

void main()
{	
	fragTexCoord = vertTexCoord;
		
	vec4 vertex = vert;	
	// Adjust z by the value in the depth map
	vertex.z = texture(depthMap, vertTexCoord).x; 

	vec4 mvVertex = modelViewMatrix * vertex;

	lightDirection = normalize(lightPosition - mvVertex.xyz);
	eyeVector = -mvVertex.xyz;

	gl_Position = projectionMatrix * mvVertex;
} 
