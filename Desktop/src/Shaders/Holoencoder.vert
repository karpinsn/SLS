uniform mat4 projectorModelView;
varying vec4 projectorPos;

void main()
{	
	//	Need Modelview for the boundingbox. That will give us world coord relative to the box
	projectorPos = gl_ProjectionMatrix * projectorModelView * vec4(gl_Vertex);

	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
} 
