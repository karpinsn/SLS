varying vec4 fragVert;

void main()
{	
	fragVert = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(gl_Vertex);
	gl_Position = fragVert;
} 
