#version 130

uniform sampler2D texture2Display;

in vec2 fragTexCoord;
out vec4 fragmentColor;

void main()
{
	vec3 tex = texture(texture2Display, fragTexCoord).rgb;
	fragmentColor = vec4(tex, 1.0);
}
