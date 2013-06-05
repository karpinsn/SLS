#version 130

precision highp float;

uniform int pitch1;
uniform int pitch2;

uniform sampler2D unfilteredWrappedPhase;
uniform sampler2D filteredWrappedPhase;

in  vec2 fragTexCoord;
out vec4 phase;

void main( void )
{
	// If the components are all zero that means we need to filter it off
	if( vec4( 0.0 ) == texture( unfilteredWrappedPhase, fragTexCoord ) )
		{ discard; }

	vec4 wPhase = texture( filteredWrappedPhase, fragTexCoord );

	float twoPi = 6.28318530718;
	float pitch12 = float( pitch1 * pitch2 ) / float( abs( pitch1 - pitch2 ) );

	float phi1 = atan( wPhase.x, wPhase.y );
	float phi2 = atan( wPhase.z, wPhase.a );
	float phi12 = mod( phi2 - phi1, twoPi );

	float k = round( ( phi12 * ( pitch12 / pitch1 ) - phi1 ) / twoPi);
	phase = vec4( vec3( phi1 + k * twoPi ), 1.0 );
}