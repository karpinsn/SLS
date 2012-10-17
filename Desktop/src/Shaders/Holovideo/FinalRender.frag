#version 130

uniform sampler2D normals;
uniform sampler2D holoImage;

in vec3 fragVert;

void main()
{
	vec3 Normal = normalize(gl_NormalMatrix * vec3(texture(normals, gl_TexCoord[0].st)));
	vec3 holoFrag = vec3(texture(holoImage, gl_TexCoord[0].st));
	vec3 L = normalize(gl_LightSource[0].position.xyz - fragVert);
	vec3 E = normalize(-fragVert);
	vec3 R = normalize(-reflect(L,Normal));
	
	//	Ambient light
	vec4 Iamb = gl_FrontLightProduct[0].ambient;
	
	//	Diffuse light
	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(Normal,L), 0.0);
	
	//	Specular light
	vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0), gl_FrontMaterial.shininess);
	
	//	Total color
	if(holoFrag.rgb == vec3(0.0))
	{
		discard;
	}
	else
	{
		gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
	}
}
