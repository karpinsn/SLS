// phase.cpp
// Developed by Song Zhang
// Copyright @ Song Zhang

#include "Capture/ComputePhase.h"

#include <iostream>
#include <cmath>
#include <assert.h>
#include <limits>

using namespace std;

CComputePhase::CComputePhase(const int & width, const int & height, const float &threshold)
:m_width(width), m_height(height), m_size(width * height), m_threshold(threshold)
{
	m_wrappedPhase = new float [m_size];
	m_wrappedMask = new unsigned char[m_size];
	m_reference  = new unsigned char[m_size];
	memset(m_reference, 0, sizeof(m_reference[0])* m_size);
}
CComputePhase::~CComputePhase()
{
	delete [] m_wrappedPhase;
	delete [] m_wrappedMask;
	delete [] m_reference;
}
void CComputePhase::RemoveBackGnd_Reference(unsigned char **fringeData, const unsigned char * textureData)
{
	static bool started = false;
	static bool isReference = false;

	if (!started)
	{
		FILE *fp;
		fopen_s(&fp, "C:\\Song\\Programs\\GeoVideo\\GeoVideo\\camera\\reference.raw", "rb");
		
		if (fp)
		{
			float * phase = nullptr;
			unsigned char *mask = nullptr;
			if (phase == nullptr) phase = new float[m_size];
			if (mask == nullptr) mask = new unsigned char [m_size];
			for (int N = 0; N < 3; N++)
			{
				for (int k = 0; k < 3; k++)
					fread(fringeData[k], sizeof(unsigned char), m_size, fp);
			}
			fclose (fp);

			m_trapLUTWrap.wrapPhase(fringeData, phase, mask, m_reference, m_width, m_height, m_threshold);

			delete [] phase;
			delete [] mask;	
			
			started = true;	
			isReference = true;
		}
	}
	if (isReference)
	{
		for (int k = 0; k < m_size; k++)
		{
			if (abs(m_reference[k] - textureData[k]) < 4)
			{
				m_wrappedMask[k] = 0;
			}
		}
	}
}

void CComputePhase::computePhase(unsigned char *fringeData[3], float *unwrappedPhase, 
						 unsigned char *unwrappedMask, unsigned char *textureData)
{

	
	memset(m_wrappedMask, 0, sizeof(unsigned char) * m_size);
	
	m_trapLUTWrap.wrapPhase(fringeData, m_wrappedPhase, m_wrappedMask, textureData, m_width, m_height, m_threshold);
	
	RemoveBackGnd_Reference(fringeData, textureData);
	
	memset(unwrappedMask, 0, sizeof(unsigned char) * m_size);
	memset(unwrappedPhase, 0, sizeof(float) * m_size);

	m_thUnwrap.ThresholdedUnrapAlg(m_wrappedPhase, unwrappedPhase, m_wrappedMask, unwrappedMask, m_width, m_height, 2);	
}
