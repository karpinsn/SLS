
#ifndef		M_PHASE_H
#define		M_PHASE_H
#include "ThresholdedPhaseUnwrap.h"
#include "TrapezoidalLUTPhaseWrap.h"

class CComputePhase
{

public:
	CComputePhase(const int & w = 256, const int & h = 256, const float &thresh = 0.7);
	~CComputePhase();

	void computePhase(unsigned char *fringe[3], float *phase, unsigned char *mask, unsigned char *texture);
	void wrapPhase(unsigned char *fringeData[3], unsigned char *textureData);
	void unwrapPhase(float *unwrappedPhase, unsigned char *unwrappedMask);

	CThresholdedPhaseUnwrap m_thUnwrap;
	CTrapezoidalLUTPhaseWrap m_trapLUTWrap;
private:

	void RemoveBackGnd_Reference(unsigned char **fringeData, const unsigned char * textureData);

	const int m_width;
	const int m_height;
	const int m_size;
	const float m_threshold;

	float *m_wrappedPhase;
	unsigned char *m_wrappedMask;
	unsigned char *m_reference;

	PhaseWrapMethod phaseWrapType;
};
#endif
