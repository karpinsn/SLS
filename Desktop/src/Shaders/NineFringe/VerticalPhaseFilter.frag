#version 130

/*
   3x3 Median Filter

Author: Nik Karpinsky

Adopted from:
A Fast, Small-Radius GPU Median Filter
by Morgan McGuire
from Shader X6 Advanced Rendering Techniques
 */

#define m2(a,b) 	t = a; a = min(t,b); b = max(t,b);
#define m3(a,b,c) 	m2(b,c); m2(a,c); m2(a,b);
#define m4(a,b,c,d) 	m2(a,b); m2(c,d); m2(a,c); m2(b,d);
#define m5(a,b,c,d,e) 	m2(a,b); m2(c,d); m2(a,c); m2(a,e); m2(d,e); m2(b,e);
#define m6(a,b,c,d,e,f)	m2(a,d); m2(b,e); m2(c,f); m2(a,b); m2(a,c); m2(e,f); m2(d,f);

uniform sampler2D image;
uniform float width;
uniform float height;

in vec2 fragTexCoord;
out vec4 filteredImage;

float step_w = 1.0/width;
float step_h = 1.0/height;

void main(void)
{
    float v[15];
    float t;


    for(int dY = -7; dY <= 7; ++dY)
    {
        vec2 offset = vec2(0.0, float(dY) * step_h);
        v[dY + 7] = texture(image, fragTexCoord + offset).x;
    }


    if(0.0 == v[0])
    {
        discard;
    }

    //  Add the extra checks for 9 - 14. Not the fastest way but it will work
    //  for right now
    m6(v[9], v[1], v[2], v[3], v[4], v[10]);
    m6(v[11], v[1], v[2], v[3], v[4], v[12]);
    m6(v[13], v[1], v[2], v[3], v[4], v[14]);
    
    m6(v[0], v[1], v[2], v[3], v[4], v[5]);
    m5(v[1], v[2], v[3], v[4], v[6]);
    m4(v[2], v[3], v[4], v[7]);
    m3(v[3], v[4], v[8]); 

    filteredImage = vec4(v[4], 0.0, 0.0, 0.0);
}
