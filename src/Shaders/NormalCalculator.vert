varying vec4 vertex;

void main() 
{
  gl_TexCoord[0] = gl_MultiTexCoord0;
  vertex = gl_Vertex;
  gl_Position = ftransform();
}