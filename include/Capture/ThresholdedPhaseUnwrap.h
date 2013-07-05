// ThresholdedPhaseUnwrap.h: interface of the CThresholdedPhaseUnwrap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <cmath>

class CThresholdedPhaseUnwrap  
{
public:
	CThresholdedPhaseUnwrap();
	virtual ~CThresholdedPhaseUnwrap();

public:

	void ThresholdedUnrapAlg(const float *wrappedPhase, float *&unwrappedPhase,
		const unsigned char *wrappedMask, unsigned char *&unwrappedMask, 
		const int &imageWidth, const int &imageHeight, const unsigned int num_steps = 6);
private:
	void ThresholdedFindStartPoint(int &startx, int &starty);
	void ThresholdedUnwrapBottomRight(const int &xstart, const int &ystart);
	void ThresholdedUnwrapTopRight(const int &xstart, const int &ystart);
	void ThresholdedUnwrapBottomLeft(const int &xstart, const int &ystart);
	void ThresholdedUnwrapTopLeft(const int &xstart, const int &ystart);

	void ThresholdedgetQualityMap();
	float Gradient(const float &phase1, const float &phase2);

	double computeAbsGradient(const double &phase1, const double &phase2)
	{
		double r = fabs(phase1 - phase2);
		return r > 0.5f? fabs(1.0f - r):r;
	}

	const float *m_fphase;
	float *m_qual_map;
	float *m_fsolution;
	double m_threshold;
	const unsigned char *m_ucmask;
	unsigned char *m_uclocalmask;

	double m_dmax, m_dmin, m_daverage, m_dsigma;
	int  m_iwidth;
	int  m_iheight;
	int  m_isize;
};
