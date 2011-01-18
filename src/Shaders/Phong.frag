varying vec3 normal;
varying vec3 v;

void main()
{
	vec3 L = normalize(gl_LightSource[0].position.xyz - v);
	vec3 E = normalize(-v);
	vec3 R = normalize(-reflect(L,normal));
	
	//	Ambient light
	vec4 Iamb = gl_FrontLightProduct[0].ambient;
	
	//	Diffuse light
	vec4 Idiff = gl_FrontLightProduct[0].diffuse * max(dot(normal,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);
	
	//	Specular light
	vec4 Ispec = gl_FrontLightProduct[0].specular * pow(max(dot(R,E),0.0), gl_FrontMaterial.shininess);
	Ispec = clamp(Ispec, 0.0, 1.0);
	
	//	Total color
	gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
}
