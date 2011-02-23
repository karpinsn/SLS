#version 330    // This is needed for the round operator

in vec3 vert;
in vec2 vertTexCoord;

out vec2 fragTexCoord;

void main()
{
        fragTexCoord = vertTexCoord;

        gl_Position = vec4(vert, 1.0);
}

