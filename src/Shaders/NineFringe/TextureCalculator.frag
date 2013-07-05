#version 130

uniform sampler2D fringe;

in vec2 fragTexCoord;
out vec4 textureMap;

void main(void)
{
	vec3 fringeImages = texture(fringe, fragTexCoord).rgb;
    
    float texColor = (fringeImages.r + fringeImages.g + fringeImages.b) / 3.0;
	textureMap = vec4(texColor, texColor, texColor, 1.0);
}
