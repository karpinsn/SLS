#version 130

uniform sampler2D normals;
uniform sampler2D holoImage;

uniform mat4 normalMatrix;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;

in vec3 lightDirection;
in vec3 fragVert;
in vec2 fragTexCoord;

out vec4 fragColor;

void main()
{
	vec3 Normal = normalize(normalMatrix * texture(normals, fragTexCoord)).xyz;
	vec3 holoFrag = vec3(texture(holoImage, fragTexCoord));
	vec3 L = normalize(lightDirection);
	vec3 E = normalize(-fragVert);
	vec3 R = normalize(-reflect(L,Normal));
	
	//	Ambient light
	vec4 Iamb = ambientColor;
	
	//	Diffuse light
	vec4 Idiff = diffuseColor * max(dot(Normal,L), 0.0);
	
	//	Specular light
	vec4 Ispec = specularColor * pow(max(dot(R,E),0.0), 128.0);
	
	//	Total color
	if(holoFrag.rgb == vec3(0.0))
	{
		discard;
	}
	else
	{
		fragColor = Iamb + Idiff + Ispec;
	}
}
