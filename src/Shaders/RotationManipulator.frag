#version 130

uniform vec4 lightPosition;
uniform vec4 ringColor;

in vec4 fragVert;
in vec4 fragNormal;

out vec4 fragColor;

void main(void)
{	

	vec4 L = normalize(lightPosition - fragVert);
	vec4 E = normalize(-fragVert);
	vec4 R = normalize(-reflect(L,fragNormal));
	
	//	Ambient light
	vec4 Iamb = 0.2 * ringColor;
	
	//	Diffuse light
	vec4 Idiff = 0.8 * ringColor * max(dot(fragNormal,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);
	
	//	Specular light
	float shininess = .2;
	vec4 Ispec = 0.8 * ringColor * pow(max(dot(R,E),0.0), shininess);
	Ispec = clamp(Ispec, 0.0, 1.0);
	
	//	Total color
	fragColor = clamp(Iamb + Idiff + Ispec, 0.0, 1.0);
}
