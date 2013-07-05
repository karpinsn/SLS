#version 130

uniform sampler2D actualPhase;
uniform sampler2D referencePhase;
uniform sampler2D textureMap;

uniform float scalingFactor;
uniform float shiftFactor;
uniform float blackLevelThreshold;

in vec2 fragTexCoord;
out vec4 depthMap;

void main(void)
{
	float aPhase = texture(actualPhase, fragTexCoord).r;
	float rPhase = texture(referencePhase, fragTexCoord).r;
    float blackLevel = texture(textureMap, fragTexCoord).r;

    if(!isinf(aPhase) && !isnan(aPhase) && blackLevel >= blackLevelThreshold)
    {
	    depthMap = vec4((rPhase - aPhase) * scalingFactor) - shiftFactor;
    }
    else
    {
        depthMap = vec4(0.0);
    }
}
