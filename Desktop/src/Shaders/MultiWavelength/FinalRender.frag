#version 120

uniform sampler2D phaseMap;
uniform sampler2D normals;

varying vec3 v;

void main()
{
	float phase = texture2D(phaseMap, gl_TexCoord[0].st).r;
	vec3 Normal = normalize(gl_NormalMatrix * vec3(texture2D(normals, gl_TexCoord[0].st)));
	vec3 L = normalize(gl_LightSource[0].position.xyz - v);
	vec3 E = normalize(-v);
	vec3 R = normalize(-reflect(L,Normal));
	
	//	Ambient light
	vec4 Iamb = gl_FrontLightProduct[0].ambient;
	
	//	Diffuse light
	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(Normal,L), 0.0);
	
	//	Specular light
	vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0), gl_FrontMaterial.shininess);

	if(0.0 == phase)
	{
		gl_FragColor = vec4(0.0);
	}
	else
	{
		gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
	}
}
