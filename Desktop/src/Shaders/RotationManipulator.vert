#version 130

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 normalMatrix;

in vec3 vert;
in vec3 vertNormal;

out vec4 fragNormal;
out vec4 fragVert;

void main()
{
    fragNormal = normalMatrix * vec4(vertNormal, 0.0);
	fragVert = modelViewMatrix * vec4(vert, 1.0);
    gl_Position = projectionMatrix * fragVert;
}