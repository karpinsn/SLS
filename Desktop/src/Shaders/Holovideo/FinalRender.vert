#version 130

uniform sampler2D depthMap;
uniform float width;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPosition;

in vec4 vert;
in vec2 vertTexCoord;

out vec3 fragVert;
out vec2 fragTexCoord;
out vec3 lightDirection;

void main()
{	
	fragTexCoord = vertTexCoord;

	vec4 newVertexPosition = vert;		
	newVertexPosition.z = texture(depthMap, vertTexCoord).x;

	fragVert = vec3(modelViewMatrix * newVertexPosition);
	lightDirection = normalize(lightPosition - fragVert);

    gl_Position = projectionMatrix * newVertexPosition;
} 
