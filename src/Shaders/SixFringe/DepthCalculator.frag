#version 130

precision highp float;
precision highp sampler2D;

uniform sampler2D actualPhase;
uniform sampler2D referencePhase;

uniform float scalingFactor;

in vec2 fragTexCoord;
out vec4 depthMap;

void main(void)
{
	vec4 aPhase = texture(actualPhase, fragTexCoord);

	// If the alpha is zero that means we need to filter it off
	if( 0.0001 >= aPhase.a )
	{ discard; }

	float rPhase = texture(referencePhase, fragTexCoord).r;
	depthMap = vec4(vec3((aPhase.r - rPhase) * scalingFactor), 1.0);
}
