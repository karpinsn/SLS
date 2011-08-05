#version 330

uniform sampler2D phaseMap;
uniform sampler2D normals;

uniform mat4 modelViewMatrix;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

in vec3 lightDirection;
in vec2 fragTexCoord;

out vec4 fragColor;

void main()
{
	float phase = texture2D(phaseMap, fragTexCoord).r;
	if(phase == 0)	// If the phase is 0 we should just get rid of this fragment
	{
		discard;
	}

	vec3 normal = normalize(modelViewMatrix * texture2D(normals, fragTexCoord)).xyz;

	//	Diffuse
	float diffuseIntensity = max(0.0, dot(normal, normalize(lightDirection)));
	fragColor = diffuseIntensity * diffuseColor;
	
	//	Ambient
	fragColor += ambientColor;

	//	Specular
	vec3 reflection = normalize(-reflect(normalize(lightDirection), normal));
	float spec = max(0.0, dot(normal, reflection));

	if(diffuseIntensity != 0.0)
	{
		float fspec = pow(spec, 128.0);
		fragColor.rgb += vec3(fspec, fspec, fspec);
	}		
}
