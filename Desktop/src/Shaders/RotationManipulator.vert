#version 130

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

in vec3 vert;
in vec3 vertNormal;

out vec3 fragNormal;

void main()
{
    fragNormal = vertNormal;
    gl_Position = projectionMatrix * modelViewMatrix * vec4(vert, 1.0);
}