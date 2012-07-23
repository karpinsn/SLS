// TrapezoidalLUTPhaseWrap.h: interface of the CTrapezoidalLUTPhaseWrap class
//
//////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PhasewrapunwrapAux.h"

class CTrapezoidalLUTPhaseWrap
{
public:
	CTrapezoidalLUTPhaseWrap();
	~CTrapezoidalLUTPhaseWrap();
	void  wrapPhase(unsigned char **fringeImage, float *&phaseMap, 
			unsigned char *&maskMap, unsigned char *&textureImage, 
			const int &imageWidth, 
			const int &imageHeight, 
			const float &gammaThreshold );
private:

	void TrapezoidalLUT();
	void computeRatioLUT(int size);

	double *m_ratioLUT;
	float  *m_phaseData;
	unsigned char *m_maskData;
	unsigned char *m_textureData;
	unsigned char **m_fringeData;
	unsigned char *m_gammaData;
	
	int m_imageWidth;
	int m_imageHeight;
	int m_imageSize;
	double m_threshold;

};
