uniform sampler2D holoImage;

varying vec3 N;
varying vec3 v;

void main()
{	
	v = vec3(gl_ModelViewMatrix * gl_Vertex);
	N = normalize(gl_NormalMatrix * gl_Normal);
	
	vec4 holoPhase = texture2DLod(holoImage, gl_MultiTexCoord0.st, 0.0);

	float P = 256.0;			// Pixels per period of the projector
	float W = 512.0;			// Width of the image
	float pixelWidth = 1.0 / W;	// Width of a pixel
	float pi = 3.14159265; 		// Mmmmmm PI
	float theta = pi / 6.0;		// Angle between camera and projector. 30 degrees
	
	float I1 = holoPhase.x * 255.0;
	float I2 = holoPhase.y * 255.0;
	float I3 = floor(holoPhase.z * 255.0 / 51.0);
	
	float phaseR = (gl_Vertex.x / pixelWidth) * ((2.0*pi*cos(theta))/P);
	float phaseA = atan((I1 - 127.5), (I2 - 127.5)) + (2.0 * pi * I3);
	
	vec4 newVertexPosition = gl_Vertex;
		

	if(I1 > 2.0 || I2 > 2.0)
	{		
		newVertexPosition.z = (phaseA - phaseR) * P / (W * sin(theta)*2.0*pi);
	}
	else 
	{
	}

	
	gl_Position = gl_ModelViewProjectionMatrix * newVertexPosition;
} 
