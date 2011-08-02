#version 120

uniform sampler2D phaseMap;
uniform sampler2D normals;

in vec4 fragVert;
in vec2 fragTexCoord;

void main()
{
	float phase = texture2D(phaseMap, fragTexCoord).r;
	vec3 Normal = normalize(gl_NormalMatrix * vec3(texture2D(normals, fragTexCoord)));

	vec3 L = normalize(gl_LightSource[0].position.xyz - fragVert.xyz);
	vec3 E = normalize(-fragVert.xyz);
	vec3 R = normalize(-reflect(L,Normal));
	
	//	Ambient light
	vec4 Iamb = gl_FrontLightProduct[0].ambient;
	
	//	Diffuse light
	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(Normal,L), 0.0);
	
	//	Specular light
	vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0), gl_FrontMaterial.shininess);

	gl_FragColor = vec4(0.0);
	
	if(0.0 != phase)
	{
		gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
	}
}
