#version 130

uniform mat3 modelViewMatrix;

in vec3 vert;
in vec3 vertNormal;

out vec3 fragNormal;

void main()
{
        fragNormal = vertNormal;
        gl_Position = vec4(modelViewMatrix * vert, 1.0);
}