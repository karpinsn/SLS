#version 130	// This is needed for the round operator

precision highp float;

uniform sampler2D fringeImage1;
uniform sampler2D fringeImage2;

in vec2 fragTexCoord;
out vec4 phase;

void main(void)
{
	float pi = 3.14159;
	float twoPi = 2.0 * pi;
	
	float pitch1 = 30;
    float pitch2 = 684;

	vec3 fringe1 = texture2D(fringeImage1, fragTexCoord).rgb;
	vec3 fringe2 = texture2D(fringeImage2, fragTexCoord).rgb;

    float sine1 = sqrt(3.0) * (fringe1.r - fringe1.b);
    float cosine1 = (fringe1.g + fringe1.g) - (fringe1.r + fringe1.b);	
	
    float sine2 = sqrt(3.0) * (fringe2.r - fringe2.b);
    float cosine2 = (fringe2.g + fringe2.g) - (fringe2.r + fringe2.b);

    float phase1 = atan(-sine1, -cosine1);
    float phaseShift = 3.0 * twoPi / 4.0;
    float phase2 = atan(-sine2 * cos(phaseShift) + cosine2 * sin(phaseShift), -cosine2 * cos(phaseShift) - sine2 * sin(phaseShift));

	float magicNumber = .36;
    float temp = (phase2 * pitch2 / pitch1 - phase1) / twoPi - magicNumber;
    float k = 0;
    
    if(temp > 0)
        k = round(temp + .5);
    else
        k = round(temp - .5);	

	phase = phase1 + twoPi * k;

	if(isnan(phi1))
	{
		phase = vec4(0.0);
	}
}

