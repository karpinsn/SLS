#version 130

precision highp float;
precision highp sampler2D;

uniform int pitch1;
uniform int pitch2;

uniform sampler2D unfilteredWrappedPhase;
uniform sampler2D filteredWrappedPhase;

uniform float m;
uniform float b;

in  vec2 fragTexCoord;
out vec4 unwrappedPhase;

void main( void )
{
	vec4 unfilteredPhases = texture( unfilteredWrappedPhase, fragTexCoord );
	// If the components are zero that means we should filter off the fragment
    if( unfilteredPhases == vec4( 0.0 ) )
        { discard; }

	vec4 wPhase = texture( filteredWrappedPhase, fragTexCoord );

	float twoPi = 6.28318530718;
	float pitch12 = float( pitch1 * pitch2 ) / float( abs( pitch1 - pitch2 ) );

	float phi1 = atan( wPhase.x, wPhase.y );
	float phi2 = atan( wPhase.z, wPhase.a );
	float phi12 = mod( phi1 - phi2, twoPi );

	// Parallel Phase unwrapping line for phi12
    bool unwrap = phi12 < ( m * fragTexCoord.x + b );
    phi12 = phi12 + int( unwrap ) * twoPi;
	float k = floor( ( phi12 * float( pitch12 / pitch1 ) - phi1 ) / twoPi );

    // 1 in the alpha component signifies that this is good phase
	unwrappedPhase = vec4( vec3( phi1 + k * twoPi ), 1.0 );
}