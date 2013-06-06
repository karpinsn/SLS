#version 330

precision highp float;

uniform sampler2D normals;

uniform mat4 normalMatrix;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform vec4 specularColor;
uniform float shininess;

in vec4 fragVert;
in vec2 fragTexCoord;
in vec3 lightDirection;
in vec3 eyeVector;

out vec4 fragColor;

void main()
{
	vec3 normal = normalize( ( normalMatrix * texture( normals, fragTexCoord ) ).xyz );
	vec3 L = normalize( lightDirection ); // Light direction
	vec3 E = normalize( eyeVector );	  // Eye direction
	vec3 R = reflect( -L, normal );       // Reflection direction

	//	Ambient
	vec4 Iamb = ambientColor;

	//	Diffuse
	vec4 Idiff = diffuseColor * max(dot(normal,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);

	//	Specular
	vec4 Ispec = specularColor * pow( max( dot( R, E ), 0.0 ), shininess );

	fragColor = Iamb + Idiff + Ispec;
}
