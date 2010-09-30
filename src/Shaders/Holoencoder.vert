uniform mat4 projectorModelView;
varying vec4 fringe;

void main()
{	
	//	Need Modelview for the boundingbox. That will give us world coord relative to the box
	vec4 v = projectorModelView * vec4(gl_Vertex);
	float x = v.x;
	
	//	Constants for encoding
	float pi = 3.14159265;
	float stepHeight = .2;
	float P = 256.0;
	float T = P/4.5;
	
	fringe = vec4((1.0 - sin(pi * 2.0 * x)) * .5, (1.0 - cos(pi * 2.0 * x)) * .5, ((floor(x) * cos(2.0*pi*x/T)) * stepHeight) + stepHeight, 1.0);
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
} 
