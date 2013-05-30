#version 130	// This is needed for the round operator

precision highp float;

uniform sampler2D fringeImage1;
uniform sampler2D fringeImage2;

uniform float gammaCutoff;

in vec2 fragTexCoord;
out vec4 phase;

void main(void)
{	
	vec3 fringe1 = texture( fringeImage1, fragTexCoord ).rgb;
	vec3 fringe2 = texture( fringeImage2, fragTexCoord ).rgb;

	float phi1S = sqrt( 3.0 ) * ( fringe1.r - fringe1.b );	
	float phi1C = 2.0 * fringe1.g - fringe1.r - fringe1.b;
	float phi2S = sqrt( 3.0 ) * ( fringe2.r - fringe2.b );
	float phi2C = 2.0 * fringe2.g - fringe2.r - fringe2.b;
 
    float gamma1 = sqrt( pow( ( 2.0 * fringe1.g - fringe1.r - fringe1.b ), 2.0 ) + 3.0 * pow( ( fringe1.r - fringe1.b ), 2.0 ) ) / ( fringe1.r + fringe1.g + fringe1.b );
    float gamma2 = sqrt( pow( ( 2.0 * fringe2.g - fringe2.r - fringe2.b ), 2.0 ) + 3.0 * pow( ( fringe2.r - fringe2.b ), 2.0 ) ) / ( fringe2.r + fringe2.g + fringe2.b );
    float gamma = min( gamma1, gamma2 );

	if( gammaCutoff <= gamma )
		{ phase = vec4( phi1S, phi1C, phi2S, phi2C ); }
    else
    	{ phase = vec4( 0.0 ); }
}