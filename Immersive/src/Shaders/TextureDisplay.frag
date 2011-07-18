#version 130

uniform sampler2D texture;

in vec2 fragTexCoord;
out vec4 fragmentColor;

void main()
{
	vec3 tex = texture2D(texture, fragTexCoord).rgb;
	fragmentColor = vec4(tex, 1.0);
}
