#version 130

/*
  11x11 Gaussian Filter

  Author: Nik Karpinsky

  Adopted from:
	Faster Gaussian Blur in GLSL
	http://xissburg.com/?p=197
*/

precision highp float;

uniform sampler2D image;
uniform float kernel[11];

in vec2 fragTexCoordOffset[11];

out vec4 filteredImage;
 
void main(void)
{
    filteredImage = vec4(0.0);
    filteredImage += texture2D(image, fragTexCoordOffset[ 0])*kernel[ 0];
    filteredImage += texture2D(image, fragTexCoordOffset[ 1])*kernel[ 1];
    filteredImage += texture2D(image, fragTexCoordOffset[ 2])*kernel[ 2];
    filteredImage += texture2D(image, fragTexCoordOffset[ 3])*kernel[ 3];
    filteredImage += texture2D(image, fragTexCoordOffset[ 4])*kernel[ 4];
    filteredImage += texture2D(image, fragTexCoordOffset[ 5])*kernel[ 5];
    filteredImage += texture2D(image, fragTexCoordOffset[ 6])*kernel[ 6];
    filteredImage += texture2D(image, fragTexCoordOffset[ 7])*kernel[ 7];
    filteredImage += texture2D(image, fragTexCoordOffset[ 8])*kernel[ 8];
    filteredImage += texture2D(image, fragTexCoordOffset[ 9])*kernel[ 9];
    filteredImage += texture2D(image, fragTexCoordOffset[10])*kernel[10];
}
