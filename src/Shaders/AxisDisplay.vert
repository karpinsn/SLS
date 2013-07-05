#version 130    // This is needed for the round operator

uniform mat3 modelViewMatrix;

in vec3 vert;
in vec3 vertColor;

out vec3 fragColor;

void main()
{
        fragColor = vertColor;

        gl_Position = vec4(modelViewMatrix * vert, 1.0);
}

