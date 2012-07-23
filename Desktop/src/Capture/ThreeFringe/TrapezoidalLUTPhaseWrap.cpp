// TrapezoidalLUTPhaseWrap.cpp: implementation of the CTrapezoidalLUTPhaseWrap class
//
///////////////////////////////////////////////////////////////////////////

#include "TrapezoidalLUTPhaseWrap.h"
#include "memory.h"
#include <cmath>

///////////////////////////////////////////////////////////////////////////
// Constrcutor
///////////////////////////////////////////////////////////////////////////

CTrapezoidalLUTPhaseWrap::CTrapezoidalLUTPhaseWrap()
{
	int lutSize = 512;
	m_ratioLUT = 0;
	computeRatioLUT(lutSize >> 1);
}

///////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////

CTrapezoidalLUTPhaseWrap::~CTrapezoidalLUTPhaseWrap()
{
	if (m_ratioLUT) 
		delete [] m_ratioLUT;
}

//////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		phase wrapping algorithm.
//	Returns :
// 
//	Parameter :
//		fringeImage == the gray scale image for three steps phase unwrap.
//			The data is arranged in a [][3] two dimensional array
//		phaseMap == reference to an array where the wrapped phase will be saved.
//			The array must be allocated before this function is called.
//		maskMap == reference to an array where the mask for wrapped will be saved.
//			The array must be allocated before this function is called.
//		textureImage == reference to an array where the texture image will be save.
//			The array must be allocated before this function is called.
//		imageWidth == the width of the image.
//		imageHeight == the height of the image.
//		gammaThreshold ==
//////////////////////////////////////////////////////////////////////////////////////////

void CTrapezoidalLUTPhaseWrap::wrapPhase( unsigned char **fringeImage, float *&phaseMap, 
						unsigned char *&maskMap, unsigned char *&textureImage, 
						const int &imageWidth, 
						const int &imageHeight, 
						const float &gammaThreshold )
{
	m_maskData = maskMap;
	m_phaseData = phaseMap;
	m_textureData = textureImage;
	m_imageWidth = imageWidth;
	m_imageHeight = imageHeight;
	m_imageSize = m_imageWidth * m_imageHeight;
	m_threshold = gammaThreshold;
	m_fringeData = fringeImage;
	if (m_maskData && m_phaseData &&
		m_fringeData && m_textureData)
	{
		TrapezoidalLUT();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		compute ratio LUT for wrapping using trapezoidal
//		algorithm for sinusoidal patterns with phase shift of 120-deg.
//	Returns :
//
//	Parameters:
//		size ==
///////////////////////////////////////////////////////////////////////////////////////////

void CTrapezoidalLUTPhaseWrap::computeRatioLUT(int size)
{
	if (m_ratioLUT) 
		delete [] m_ratioLUT;

	m_ratioLUT = new double [size];
	double *ratio = new double [size];
	
	double I1, I2, I3, temp;
	int i = 0;
	memset(m_ratioLUT, 0, sizeof(m_ratioLUT[0]) * size);

	double one_over_size = one/size;

	double temp2 = one_over_size/6.0f * two_pi;
	double two_pi_over_3 = two_pi/3.0f;
	double four_pi_over_3 = 2 * two_pi_over_3;

	for ( i = 0; i < size; i++)
	{
		temp = temp2 * i;
		I1 = one + cos(temp);
		I2 = one + cos(temp + two_pi_over_3);
		I3 = one + cos(temp + four_pi_over_3);

		ratio[i] = (I3 - I2)/(I1 - I2);
	}
	
	// construct lut
	for ( i = 0; i < size; i++)
	{
		double r = one_over_size * i;
		// search for the value
		for ( int j = 0; j < size - 1; j++)
		{
	//		if ( j + 1 < size)// redudent instruction
			if ( r >= ratio[j] && r <= ratio[j+1])
			{
				double t = (r - ratio[j]) / (ratio[j + 1] - ratio[j]);
		//		m_ratioLUT[i] = r - (float) ((1-t) * j + t *(j+1)) * one_over_size;
				m_ratioLUT[i] = r - (j + t) * one_over_size;
				break;					
			}
		}
	}
	delete [] ratio;	
}

//////////////////////////////////////////////////////////////////////////////////////////
//	Description :
//		trapezoidal phase-shifting algorithm
//		to wrap sinusoidal pattern for three step
//		algorithm
//////////////////////////////////////////////////////////////////////////////////////////
void CTrapezoidalLUTPhaseWrap::TrapezoidalLUT()
{
	double convertC = -1.0/6.0;
	double maskLevel2 = m_threshold*m_threshold;

	int I1, I2, I3, Ip, t, C;
	double S, numGamma2, denGamma2threshold, tmp ;

	for ( int k = m_imageSize - 1; k >= 0; k--)
	{
		I1 = m_fringeData[0][k];	// fringe0
		I2 = m_fringeData[1][k];	// fringe120
		I3 = m_fringeData[2][k];	// fringe240
		Ip = I1 + I2 + I3;			// 3I'
		S = sqrt_3 * (I1 - I3);		// sine	
		C = (I2<<1) - (I1 + I3);		// cosine
		numGamma2 = C * C + S * S;
		denGamma2threshold = maskLevel2 * Ip * Ip ;
		t = Ip >> 1; //Real2Int(0.5f*(Ip));
		if (t > 255) t = 255;
		m_textureData[k] = t;
		//m_textureData[k] = I1;
		//m_textureData[k] = (int)(sqrt(numGamma2 / Ip / Ip) * 255);

		if (numGamma2 > denGamma2threshold)
		{
			if ( I1 >= I2 && I1 >= I3 ) // N = 2 or 3
			{
				if (I2 >= I3) // N = 2
				{
					tmp = ((double) (I2 - I3)) / (I1 - I3);
					m_phaseData[k] = float((2.0 - tmp + m_ratioLUT[Real2Int(tmp*255)])  * convertC);
					
				}
				else	// N = 3
				{
					tmp = ((double) (I3 - I2)) / (I1 - I2);
					m_phaseData[k] = float((2.0 + tmp - m_ratioLUT[Real2Int(tmp*255)])  * convertC);
				}
			}
			else if (I2 >= I1 && I2 >= I3) // N = 1 or 6
			{
				if (I1 >= I3) // N = 1
				{
					tmp = ((double) (I1 - I3)) / (I2 - I3);
					m_phaseData[k] = float((tmp - m_ratioLUT[Real2Int(tmp*255)])  * convertC);
				}
				else // N = 6
				{
					tmp = ((double) (I3 - I1)) / (I2 - I1);
					m_phaseData[k] = float((6.0 - tmp + m_ratioLUT[Real2Int(tmp*255)])  * convertC);
					
				}
			}
			else // N = 4 or 5
			{
				if (I1 >= I2) // N = 4
				{
					tmp = ((double) (I1 - I2)) / (I3 - I2);
					m_phaseData[k] = float((4.0 - tmp + m_ratioLUT[Real2Int(tmp*255)])  * convertC);
				}
				else // N = 5
				{
					tmp = ((double) (I2 - I1)) / (I3 - I1);
					m_phaseData[k] = float((4.0 + tmp - m_ratioLUT[Real2Int(tmp*255)])  * convertC);
				}
			}
			m_maskData[k] = 255;
		}
	}
}
