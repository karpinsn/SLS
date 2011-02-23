/*
  3x3 Median Filter

  Author: Nik Karpinsky

  Adopted from:
    A Fast, Small-Radius GPU Median Filter
    by Morgan McGuire
    from Shader X6 Advanced Rendering Techniques
*/

#define m2(a,b) 		t = a; a = min(t,b); b = max(t,b);
#define m3(a,b,c) 	m2(b,c); m2(a,c); m2(a,b);
#define m4(a,b,c,d) 	m2(a,b); m2(c,d); m2(a,c); m2(b,d);
#define m5(a,b,c,d,e) 	m2(a,b); m2(c,d); m2(a,c); m2(a,e); m2(d,e); m2(b,e);
#define m6(a,b,c,d,e,f)	m2(a,d); m2(b,e); m2(c,f); m2(a,b); m2(a,c); m2(e,f); m2(d,f);

uniform sampler2D image;
uniform float width;
uniform float height;

float step_w = 1.0/width;
float step_h = 1.0/height;

void main(void)
{
  float v[9];
  float t;

  for(int dX = -1; dX <= 1; ++dX)
  {
    for(int dY = -1; dY <= 1; ++dY)
    {
      v[dX * 3 + dY + 4] = texture2D(image, gl_TexCoord[0].xy + vec2(float(dX)/width, float(dY)/height)).x;
    }
  }

  m6(v[0], v[1], v[2], v[3], v[4], v[5]);
  m5(v[1], v[2], v[3], v[4], v[6]);
  m4(v[2], v[3], v[4], v[7]);
  m3(v[3], v[4], v[8]);
   
  gl_FragData[0] = vec4(v[4], 0.0, 0.0, 0.0);
}