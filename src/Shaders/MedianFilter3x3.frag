/*
  3x3 Median Filter

  Author: Nik Karpinsky

  Adopted from:
    Median Finding on a 3-by-3 Grid
    by Alan Paeth
    from "Graphics Gems", Academic Press, 1990
*/

#define s2(a,b) {float t; if ((t=b-a)<0.0) {a+=t; b-=t;}}
#define mn3(a,b,c) s2(a,b); s2(a,c);
#define mx3(a,b,c) s2(b,c); s2(a,c);
#define mnmx3(a,b,c) mx3(a,b,c); s2(a,b);
#define mnmx4(a,b,c,d) s2(a,b); s2(c,d); s2(a,c); s2(b,d);
#define mnmx5(a,b,c,d,e) s2(a,b); s2(c,d); mn3(a,c,e); mx3(b,d,e);
#define mnmx6(a,b,c,d,e,f) s2(a,d); s2(b,e); s2(c,f); mn3(a,b,c); mx3(d,e,f);

uniform sampler2D image;
uniform float width;
uniform float height;

float step_w = 1.0/width;
float step_h = 1.0/height;

void main(void)
{
  float step_w = 1.0/512.0;
  float step_h = 1.0/512.0;

  float block[6];

  block[0] = texture2D(image, gl_TexCoord[0].xy + vec2(-step_w, -step_w)).x;
  block[1] = texture2D(image, gl_TexCoord[0].xy + vec2( 0.0, -step_w)).x;
  block[2] = texture2D(image, gl_TexCoord[0].xy + vec2(+step_w, -step_w)).x;
  block[3] = texture2D(image, gl_TexCoord[0].xy + vec2(-step_w,  0.0)).x;
  block[4] = texture2D(image, gl_TexCoord[0].xy + vec2( 0.0,  0.0)).x;
  block[5] = texture2D(image, gl_TexCoord[0].xy + vec2(+step_w,  0.0)).x;

  mnmx6(block[0], block[1], block[2], block[3], block[4], block[5]);
  block[0] = texture2D(image, gl_TexCoord[0].xy + vec2(-step_w, +step_w)).x;
  mnmx5(block[0], block[1], block[2], block[3], block[4]);
  block[0] = texture2D(image, gl_TexCoord[0].xy + vec2( 0.0, +step_w)).x;
  mnmx4(block[0], block[1], block[2], block[3]);
  block[0] = texture2D(image, gl_TexCoord[0].xy + vec2(+step_w, +step_w)).x;
  mnmx3(block[0], block[1], block[2]);
   
  gl_FragData[0] = vec4(block[1], 0.0, 0.0, 0.0);
}