#version 130

uniform sampler2D actualPhase;
uniform sampler2D referencePhase;

uniform float scalingFactor;
uniform float shiftFactor;

in vec2 fragTexCoord;
out vec4 depthMap;

void main(void)
{
	float aPhase = texture(actualPhase, fragTexCoord).r;
	float rPhase = texture(referencePhase, fragTexCoord).r;

    if(!isinf(aPhase) && !isnan(aPhase))
    {
	    depthMap = vec4((rPhase - aPhase) * scalingFactor) - shiftFactor;
    }
    else
    {
        depthMap = vec4(0.0);
    }
}
