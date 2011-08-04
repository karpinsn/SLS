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
	vec3 normal = normalize(modelViewMatrix * texture2D(normals, fragTexCoord)).xyz;

	//	Diffuse
	float diffuseIntensity = max(0.0, dot(normal, normalize(lightDirection)));
	fragColor = diffuseIntensity * diffuseColor;
	
	//	Ambient
	fragColor += ambientColor;

	//	Specular
	vec3 reflection = normalize(reflect(-normalize(lightDirection), normal));
	float spec = max(0.0, dot(normal, reflection));

	if(diffuseIntensity != 0)
	{
		float fspec = pow(spec, 128.0);
		fragColor.rgb += vec3(fspec, fspec, fspec);
	}

	fragColor = vec4(0.0);
	fragColor.x = fragTexCoord.x;
	//fragColor.x = .4;

	//vec3 L = normalize(gl_LightSource[0].position.xyz - fragVert.xyz);
	//vec3 E = normalize(-fragVert.xyz);
	//vec3 R = normalize(-reflect(L,Normal));
	
	//	Ambient light
	//vec4 Iamb = gl_FrontLightProduct[0].ambient;
	
	//	Diffuse light
	//vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(Normal,L), 0.0);
	
	//	Specular light
	//vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0), gl_FrontMaterial.shininess);

	//fragColor = vec4(fragTexCoord.s);
	
	//if(0.0 != phase)
	//{
		//fragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
	//}
}
