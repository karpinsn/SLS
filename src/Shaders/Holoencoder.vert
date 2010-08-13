uniform mat4 projectorModelView;
varying float index;

void main()
{	
	//	Need Modelview for the boundingbox. That will give us world coord relative to the box
	vec4 v = projectorModelView * vec4(gl_Vertex);
	index = v.x;
					
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
} 
