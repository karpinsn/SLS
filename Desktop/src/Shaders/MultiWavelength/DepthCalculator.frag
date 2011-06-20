#version 130

uniform sampler2D actualPhase;
uniform sampler2D referencePhase;

uniform float scalingFactor;

out vec4 depthMap;

void main(void)
{
	depthMap = (actualPhase - referencePhase) * scalingFactor;
}
