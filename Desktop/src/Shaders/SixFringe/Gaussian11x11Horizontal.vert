#version 130

uniform float width;
uniform float height;

in vec3 vert;
in vec2 vertTexCoord;

out vec2 fragTexCoordOffset[11];

float step_w = 1.0/width;
float step_h = 1.0/height;

void main()
{
		fragTexCoordOffset[ 0] = vertTexCoord + vec2(5.0 * -step_w, 0.0);
		fragTexCoordOffset[ 1] = vertTexCoord + vec2(4.0 * -step_w, 0.0);
		fragTexCoordOffset[ 2] = vertTexCoord + vec2(3.0 * -step_w, 0.0);
		fragTexCoordOffset[ 3] = vertTexCoord + vec2(2.0 * -step_w, 0.0);
		fragTexCoordOffset[ 4] = vertTexCoord + vec2(1.0 * -step_w, 0.0);
		fragTexCoordOffset[ 5] = vertTexCoord;
		fragTexCoordOffset[ 6] = vertTexCoord + vec2(1.0 * step_w, 0.0);
		fragTexCoordOffset[ 7] = vertTexCoord + vec2(2.0 * step_w, 0.0);
		fragTexCoordOffset[ 8] = vertTexCoord + vec2(3.0 * step_w, 0.0);
		fragTexCoordOffset[ 9] = vertTexCoord + vec2(4.0 * step_w, 0.0);
		fragTexCoordOffset[10] = vertTexCoord + vec2(5.0 * step_w, 0.0);
		
        gl_Position = vec4(vert, 1.0);
}
