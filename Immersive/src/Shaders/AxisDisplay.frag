#version 130

in vec3 fragColor;
out vec4 color;

void main(void)
{	
	color = vec4(fragColor, 0.0);
}
