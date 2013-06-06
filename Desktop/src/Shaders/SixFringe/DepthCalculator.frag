#version 130

precision highp float;

uniform sampler2D actualPhase;
uniform sampler2D referencePhase;

uniform float scalingFactor;

in vec2 fragTexCoord;
out vec4 depthMap;

void main(void)
{
	float aPhase = texture(actualPhase, fragTexCoord).r;
	float rPhase = texture(referencePhase, fragTexCoord).r;
	depthMap = vec4(vec3((aPhase - rPhase) * scalingFactor), 1.0);
}
