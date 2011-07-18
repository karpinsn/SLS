#version 130

uniform sampler2D actualPhase;
uniform sampler2D referencePhase;

uniform float scalingFactor;

in vec2 fragTexCoord;
out vec4 depthMap;

void main(void)
{
	float aPhase = texture2D(actualPhase, fragTexCoord).r;
	float rPhase = texture2D(referencePhase, fragTexCoord).r;

	depthMap = vec4((aPhase - rPhase) * scalingFactor);
}
