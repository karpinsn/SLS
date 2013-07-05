// ThresholdedPhaseUnwrap.cpp: implementation of the CThresholdedPhaseUnwrap class.
//
//////////////////////////////////////////////////////////////////////

#include "Capture/phasewrapunwrapAux.h"
#include "Capture/ThresholdedPhaseUnwrap.h"
#include "Capture/FastConnectComponent.h"
#include <vector>
#include <cmath>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

using namespace std;

//////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////

CThresholdedPhaseUnwrap::CThresholdedPhaseUnwrap()
{
	m_iwidth = 256;
	m_iheight = 256;
	m_fphase = nullptr;
	m_fsolution = nullptr;
	m_ucmask = nullptr;
	m_uclocalmask = nullptr;
	m_qual_map =  nullptr;
}

//////////////////////////////////////////////////////////////////////
//
// Destructor
//
//////////////////////////////////////////////////////////////////////

CThresholdedPhaseUnwrap::~CThresholdedPhaseUnwrap()
{
	if (m_qual_map) delete [] m_qual_map;
	m_qual_map = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
//   
// Returns:
//
// Parameters:
//   wrappedPhase == reference to wrapped phase
//   unwrappedPhase == reference to an array where the calculated unwrapped phase will be saved.
//			The array must be allocated before this function is called.
//   wrappedMask == mask for wrapped phase.
//   unwrappedMask == reference to an array where the mask for unwrapped phase will be saved.
//			This array must be allocated before this function is called.
//   imageWidth == the width of the image.
//   imageHeight == the height of the image.
//   qua_map == reference to an array where the quality map will be saved. This array must be 
//			allocated before this function is called.
//	 num_steps == define how many steps will be used to unwrapped the phase.
//
//////////////////////////////////////////////////////////////////////////////////

void CThresholdedPhaseUnwrap::ThresholdedUnrapAlg(const float *wrappedPhase, float *&unwrappedPhase,
		const unsigned char *wrappedMask, unsigned char *&unwrappedMask, 
		const int &imageWidth, 	const int &imageHeight, const unsigned int num_steps )
{
	m_iwidth = imageWidth;
	m_iheight = imageHeight;
	m_isize = imageWidth * imageHeight;
	m_fphase = wrappedPhase;
	m_fsolution = unwrappedPhase;
	m_ucmask = wrappedMask;
	m_uclocalmask = unwrappedMask;
	if (m_qual_map == nullptr)
	{
		m_qual_map =  new float [m_iwidth * m_iheight];
	}

	CFastConnectComponent fst;
	memset(m_uclocalmask, 0, sizeof(m_uclocalmask[0])*m_isize);

	int unwrapStartPointIndex = fst.FastConnectAlg(m_ucmask, m_uclocalmask, m_iwidth, m_iheight, 0x00, 0x255);

	ThresholdedgetQualityMap();

	memset(m_uclocalmask, 0, sizeof(m_uclocalmask[0])*m_isize);

	m_threshold = m_daverage;

	int istartx = unwrapStartPointIndex % m_iwidth;
	int istarty = unwrapStartPointIndex / m_iwidth;

	int k = unwrapStartPointIndex;
	if( m_qual_map[k] > m_threshold)
	{
		m_uclocalmask[k] = 255;
		m_fsolution[k] = m_fphase[k];
	}
	else
	{
		istartx = m_iwidth >> 1;
		istarty = m_iheight >> 1;
		ThresholdedFindStartPoint(istartx, istarty);
		// set starting point as unwrapped
		k = istarty * m_iwidth + istartx;
		m_uclocalmask[k] = 255;
		m_fsolution[k] = m_fphase[k];
	}

	m_threshold -= 1.5*m_dsigma;
	for( unsigned int i = 0; i < num_steps; i++)
	{
		ThresholdedUnwrapTopLeft(istartx, istarty);
		ThresholdedUnwrapBottomRight(istartx, istarty);
		ThresholdedUnwrapTopRight(istartx, istarty);
		ThresholdedUnwrapBottomLeft(istartx, istarty);
		// song
		if (i == num_steps - 2) m_dsigma = 20;
		m_threshold -= m_dsigma;
		m_dsigma *= 2;		
		
	}
	
	for (int i = 0; i < m_iwidth * m_iheight; i++)
	{
		m_fsolution[i] *= (float)two_pi;
	}
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
// 
// Returns:
//
// Parameters:
//   startx ==
//   starty ==
//
//////////////////////////////////////////////////////////////////////////////////

void CThresholdedPhaseUnwrap::ThresholdedFindStartPoint(int &startx, int &starty)
{

	// Right:  x++
	// Left :  x--
	// Bottom: y++
	// Top   : y--

	// Search the bottom right part for a start point y++, x++
	for (int j = starty; j < m_iheight - 1; j++)
	{
		int k = j * m_iwidth;
		for ( int i = startx; i < m_iwidth - 1; i++)
		{
			//if( m_qual_map[ k + i] <= m_threshold) continue;
			if ((m_ucmask[ k + i] & // current pixel
				m_ucmask[ k + i + 1] & //right neighbor
				m_ucmask[ k + i - 1] & //left point
				m_ucmask[ k + m_iwidth + i] & // bottom neighbor
				m_ucmask[ k - m_iwidth + i]&  // top neighbor
				m_ucmask[ k - m_iwidth + i + 1] & // top right
				m_ucmask[ k - m_iwidth + i - 1] & // top left
				m_ucmask[ k + m_iwidth + i + 1] & // bottom right
				m_ucmask[ k + m_iwidth + i - 1])&&
				(m_qual_map[ k + i] > m_threshold && // current pixel
				m_qual_map[ k + i + 1] > m_threshold&& //right neighbor
				m_qual_map[ k + i - 1] > m_threshold &&//left point
				m_qual_map[ k + m_iwidth + i] > m_threshold && // bottom neighbor
				m_qual_map[ k - m_iwidth + i] > m_threshold&&  // top neighbor
				m_qual_map[ k - m_iwidth + i + 1] > m_threshold && // top right
				m_qual_map[ k - m_iwidth + i - 1] > m_threshold && // top left
				m_qual_map[ k + m_iwidth + i + 1] > m_threshold && // bottom right
				m_qual_map[ k + m_iwidth + i - 1] > m_threshold)
				) // bottom left

			{
				startx = i;
				starty = j;
				return;
			}
		}
	}

	// Search the top right part for a start point, y--,x++
	for (int j = starty; j > 1; j--)
	{
		int k = j * m_iwidth;
		for ( int i = startx; i < m_iwidth - 1; i++)
		{
				//if( m_qual_map[ k + i ] <= m_threshold) continue;
				if ((m_ucmask[ k + i] & // current pixel
				m_ucmask[ k + i + 1] & //right neighbor
				m_ucmask[ k + i - 1] & //left point
				m_ucmask[ k + m_iwidth + i] & // bottom neighbor
				m_ucmask[ k - m_iwidth + i]&  // top neighbor
				m_ucmask[ k - m_iwidth + i + 1] & // top right
				m_ucmask[ k - m_iwidth + i - 1] & // top left
				m_ucmask[ k + m_iwidth + i + 1] & // bottom right
				m_ucmask[ k + m_iwidth + i - 1]) // bottom left
				&&
				(m_qual_map[ k + i]  > m_threshold && // current pixel
				m_qual_map[ k + i + 1]  > m_threshold && //right neighbor
				m_qual_map[ k + i - 1]  > m_threshold && //left point
				m_qual_map[ k + m_iwidth + i]  > m_threshold && // bottom neighbor
				m_qual_map[ k - m_iwidth + i] > m_threshold &&  // top neighbor
				m_qual_map[ k - m_iwidth + i + 1]  > m_threshold && // top right
				m_qual_map[ k - m_iwidth + i - 1]  > m_threshold && // top left
				m_qual_map[ k + m_iwidth + i + 1] > m_threshold && // bottom right
				m_qual_map[ k + m_iwidth + i - 1] > m_threshold) // bottom left
				)
			{
				startx = i;
				starty = j;
				return;
			}
		}
	}

	// Search the bottom left part for a start point, y++,x--
	for (int j = starty; j < m_iheight - 1; j++)
	{
		int k = j * m_iwidth;
		for ( int i = startx; i > 0; i--)
		{
			//if( m_qual_map[ k + i] <= m_threshold) continue;
			if ((m_ucmask[ k + i] & // current pixel
				m_ucmask[ k + i + 1] & //right neighbor
				m_ucmask[ k + i - 1] & //left point
				m_ucmask[ k + m_iwidth + i] & // bottom neighbor
				m_ucmask[ k - m_iwidth + i]&  // top neighbor
				m_ucmask[ k - m_iwidth + i + 1] & // top right
				m_ucmask[ k - m_iwidth + i - 1] & // top left
				m_ucmask[ k + m_iwidth + i + 1] & // bottom right
				m_ucmask[ k + m_iwidth + i - 1]) // bottom left
				&&
				(m_qual_map[ k + i]  > m_threshold && // current pixel
				m_qual_map[ k + i + 1]  > m_threshold && //right neighbor
				m_qual_map[ k + i - 1]  > m_threshold && //left point
				m_qual_map[ k + m_iwidth + i]  > m_threshold && // bottom neighbor
				m_qual_map[ k - m_iwidth + i] > m_threshold &&  // top neighbor
				m_qual_map[ k - m_iwidth + i + 1]  > m_threshold && // top right
				m_qual_map[ k - m_iwidth + i - 1]  > m_threshold && // top left
				m_qual_map[ k + m_iwidth + i + 1] > m_threshold && // bottom right
				m_qual_map[ k + m_iwidth + i - 1]> m_threshold) // bottom left
				)
			{
				startx = i;
				starty = j;
				return;
			}
		}
	}

	// Search the top left part for a start point, y--,x--
	for (int j = starty; j > 1; j--)
	{
		int k = j * m_iwidth;
		for ( int i = startx; i > 1; i--)
		{
			//if( m_qual_map[ k + i ] <= m_threshold) continue;
			if ((m_ucmask[ k + i] & // current pixel
				m_ucmask[ k + i + 1] & //right neighbor
				m_ucmask[ k + i - 1] & //left point
				m_ucmask[ k + m_iwidth + i] & // bottom neighbor
				m_ucmask[ k - m_iwidth + i]&  // top neighbor
				m_ucmask[ k - m_iwidth + i + 1] & // top right
				m_ucmask[ k - m_iwidth + i - 1] & // top left
				m_ucmask[ k + m_iwidth + i + 1] & // bottom right
				m_ucmask[ k + m_iwidth + i - 1]) // bottom left
				&&
				(m_qual_map[ k + i]  > m_threshold && // current pixel
				m_qual_map[ k + i + 1]  > m_threshold && //right neighbor
				m_qual_map[ k + i - 1]  > m_threshold && //left point
				m_qual_map[ k + m_iwidth + i]  > m_threshold && // bottom neighbor
				m_qual_map[ k - m_iwidth + i] > m_threshold &&  // top neighbor
				m_qual_map[ k - m_iwidth + i + 1]  > m_threshold && // top right
				m_qual_map[ k - m_iwidth + i - 1]  > m_threshold && // top left
				m_qual_map[ k + m_iwidth + i + 1] > m_threshold && // bottom right
				m_qual_map[ k + m_iwidth + i - 1]> m_threshold) // bottom left
				)
			{
				startx = i;
				starty = j;
				return;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
// 
// Returns:
//
// Parameters:
//   startx ==
//   starty ==
//
//////////////////////////////////////////////////////////////////////////////////

void CThresholdedPhaseUnwrap::ThresholdedUnwrapBottomRight(const int &xstart, const int &ystart)
{
	int i,j,k,m;
	vector<int> ipossible;
	int lineindex = ystart* m_iwidth;

	// Scan the bottom-right part of the start point
	for(j = ystart ; j < m_iheight - 1; j++)
	{
		lineindex = j * m_iwidth;
		for( i = xstart ; i < m_iwidth -1; i++)
		{
			k =  lineindex + i;
			if( (m_ucmask[k] ^ m_uclocalmask[k]) && m_qual_map[k] > m_threshold )
			{
				m = k - m_iwidth;
				if( m_uclocalmask[m])// This point has a neighbor with unwrapped phase on its top
				{
					// Calculate the unwrapped phase of current point
					m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k],m_fphase[m]);
					// Set the mask
					m_uclocalmask[k] = 255;

				}
				else if(m_uclocalmask[k - 1])// This point has a neighbor with know unwrapped phase on its left
				{
					// Calculate the unwrapped phase of current point
					m_fsolution[k] = m_fsolution[k - 1] + Gradient(m_fphase[k], m_fphase[k - 1]);
					m_uclocalmask[k] = 255;
				}
				//else if(m_ucmask[k + m_iwidth] || m_ucmask[k + 1])||m_ucmask[k - 1 + m_iwidth])
				// "|" is used instead of "||" to optimize in speed
				// bottom, right, bottom left
				else if(m_ucmask[k + m_iwidth] | m_ucmask[k + 1] | 
					m_ucmask[k - 1 + m_iwidth])
				{
					ipossible.push_back(k);
				}
			}
		}
	}
	// Then deal with the points that have neighbor with known phase on the right, bottom or bottom left
	for (i = (int)ipossible.size(); i > 0 ; i--)
	{
		k = ipossible[i - 1];
		m = k + 1; // known neighbor on the right
		if(m_uclocalmask[m])
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
	
			continue;
		}
		m = k + m_iwidth; // known neighbor at the bottom
		if(m_uclocalmask[m])
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
			continue;
		}

		m = k + m_iwidth - 1;
		if(m_uclocalmask[m])// known neighbor on bottom left
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
		}
	} 
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
// 
// Returns:
//
// Parameters:
//   startx ==
//   starty ==
//
//////////////////////////////////////////////////////////////////////////////////

void CThresholdedPhaseUnwrap::ThresholdedUnwrapTopRight(const int &xstart, const int &ystart)
{
	int i, j, k, m;
	vector<int> ipossible;

	// Scan Top-right part to the start point
	for(j = ystart ; j > 1; j--)
	{
		int lineindex = j * m_iwidth;
		for( i = xstart ; i < m_iwidth -1; i++)
		{
			k = lineindex + i;
			if( (m_ucmask[k] ^ m_uclocalmask[k]) && m_qual_map[k] > m_threshold)
			{
				m = k + m_iwidth;
				if( m_uclocalmask[m])// This point has a neighbor with unwrapped phase at its bottom
				{
					// Calculate the unwrapped phase of current point
					m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k],m_fphase[m]);
					// Set the mask
					m_uclocalmask[k] = 255;
				}
				else if(m_uclocalmask[k - 1])// This point has a neighbor with know unwrapped phase on its left
				{
					// Calculate the unwrapped phase of current point
					m_fsolution[k] = m_fsolution[k - 1] + Gradient(m_fphase[k], m_fphase[k - 1]);
					m_uclocalmask[k] = 255;
				}
				// "|" is used instead of "||" to optimize in speed
				// top, right, top left
				else if(m_ucmask[k - m_iwidth] | m_ucmask[k + 1] |
					m_ucmask[k - 1 - m_iwidth])
				{
					ipossible.push_back(k);
				}
			}
		}
	}
	// Then deal with the points that have neighbor with known phase on the top, right, or top left
	for (i = (int)ipossible.size(); i > 0 ; i--)
	{
		k = ipossible[i - 1];
		m = k + 1; // known neighbor on the right
		if(m_uclocalmask[m])
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
			continue;
		}
		m = k - m_iwidth; // known neighbor on the top
		if(m_uclocalmask[m])
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
			continue;
		}
		m = k - 1 - m_iwidth; // known neighbor on the top left
		if(m_uclocalmask[m])
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
		}
	} 
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
// 
// Returns:
//
// Parameters:
//   startx ==
//   starty ==
//
//////////////////////////////////////////////////////////////////////////////////

void CThresholdedPhaseUnwrap::ThresholdedUnwrapBottomLeft(const int &xstart, const int &ystart)
{
	int i,j,k,m;
	vector<int> ipossible;

	// Scan the Bottom Left part to the start point
	for(j = ystart ; j < m_iheight - 1; j++)
	{
		int lineindex = j * m_iwidth;
		for( i = xstart; i > 1; i--)
		{
			k = lineindex + i;
			if( (m_ucmask[k] ^ m_uclocalmask[k]) && m_qual_map[k] > m_threshold)
			{
				m = k - m_iwidth;
				if( m_uclocalmask[m] )// This point has a neighbor with unwrapped phase at its top
				{
					// Calculate the unwrapped phase of current point
					m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k],m_fphase[m]);
					// Set the mask
					m_uclocalmask[k] = 255;
				}
				else if(m_uclocalmask[k + 1])// This point has a neighbor with know unwrapped phase on its right
				{
					// Calculate the unwrapped phase of current point
					m_fsolution[k] = m_fsolution[k + 1] + Gradient(m_fphase[k], m_fphase[k + 1]);
					m_uclocalmask[k] = 255;
				}
				//else if(m_ucmask[k + m_iwidth] | m_ucmask[k - 1])
				// bottom, left, bottom right
				else if(m_ucmask[k + m_iwidth] | m_ucmask[k - 1] | 
					m_ucmask[k + m_iwidth + 1]) // considering 6 neighbors 
				{
					ipossible.push_back(k);
				}
			}
		}
	}
	// Then deal with the points that have neighbor with known phase on the right or top
	for (i = (int)ipossible.size(); i > 0 ; i--)
	{
		k = ipossible[i - 1];
		m = k -1;
		if(m_uclocalmask[m])// known neighbor on the left
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
			continue;
		}
		m = k + m_iwidth; // known neighbor on the bottom
		if(m_uclocalmask[m])
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
			continue;
		}
		m = k + m_iwidth + 1;	// know neighbor on the bottom right
		if(m_uclocalmask[m])
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
		}
	} 
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
// 
// Returns:
//
// Parameters:
//   startx ==
//   starty ==
//
//////////////////////////////////////////////////////////////////////////////////

void CThresholdedPhaseUnwrap::ThresholdedUnwrapTopLeft(const int &xstart, const int &ystart)
{
	int i,j,k,m;
	vector<int> ipossible;

	// Scan the Top-Left part to the start point
	for(j = ystart; j > 1; j--)
	{
		int lineindex = j * m_iwidth;
		for( i = xstart ; i > 1; i--)
		{
			k = lineindex + i;
			if( (m_ucmask[k] ^ m_uclocalmask[k]) && m_qual_map[k] > m_threshold)
			{
				m = k + m_iwidth;
				if( m_uclocalmask[m] )// This point has a neighbor with unwrapped phase at its bottom
				{
					// Calculate the unwrapped phase of current point
					m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k],m_fphase[m]);
					// Set the mask
					m_uclocalmask[k] = 255;
				}
				else if(m_uclocalmask[k + 1])// This point has a neighbor with know unwrapped phase on its right
				{
					// Calculate the unwrapped phase of current point
					m_fsolution[k] = m_fsolution[k + 1] + Gradient(m_fphase[k], m_fphase[k + 1]);
					m_uclocalmask[k] = 255;
				}
				// top, left, top right
				else if(m_ucmask[k - m_iwidth] | m_ucmask[k - 1] |
					m_ucmask[k- m_iwidth + 1])
				{
					ipossible.push_back(k);
				}
			}
		}
	}
	// Then deal with the points that have neighbor with known phase on the top, left, or top right
	for (i = (int)ipossible.size(); i > 0 ; i--)
	{
		k = ipossible[i - 1];
		m = k - 1;	// know neighbor on the left
		if(m_uclocalmask[m])
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
			continue;
		}
		m = k - m_iwidth; // know neighbor on the top
		if(m_uclocalmask[m])
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
			continue;
		}
		m = k - m_iwidth + 1; // know neighbor on the top right
		if(m_uclocalmask[m])
		{
			m_fsolution[k] = m_fsolution[m] + Gradient(m_fphase[k], m_fphase[m]);
			m_uclocalmask[k] = 255;
		}
	} 
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
// 
// Returns:
//	 float == 
// Parameters:
//   phase1 ==
//   phase2 ==
//
//////////////////////////////////////////////////////////////////////////////////

inline float CThresholdedPhaseUnwrap::Gradient(const float &phase1, const float &phase2)
{
	float diff = phase1 - phase2;
	//return diff > 0.5 ? diff - 1.0f : (diff < -0.5 ? diff + 1.0f: diff);
	if( diff > 0.5 ) diff -= 1.0;
	else if( diff < -0.5) diff += 1.0;
	return diff;
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
//		Calculate a quality map for phase unwrap.
// 
// Returns:
// Parameters:
//
//////////////////////////////////////////////////////////////////////////////////

void CThresholdedPhaseUnwrap::ThresholdedgetQualityMap()
{
	m_daverage = 0.0;
	m_dsigma = 0.0;
	double summed_square = 0;
	int  nnn = 0;
	int i, j, k, m;
	for ( j = 1; j < m_iheight - 1; j++)
	{
		m = j * m_iwidth;
		for ( i = 1; i < m_iwidth - 1; i++)
		{
			k = m + i;
			if ( !m_uclocalmask[k]) continue;			
			m_qual_map[k] = float(- computeAbsGradient(m_fphase[k+1], m_fphase[k]) - computeAbsGradient(m_fphase[k + m_iwidth], m_fphase[k]));
			m_daverage += m_qual_map[k];
			summed_square += m_qual_map[k] * m_qual_map[k];
			nnn ++;
		}
	}
	if( nnn > 2)
	{
		m_daverage = m_daverage/nnn;
		m_dsigma = (summed_square - nnn * m_daverage * m_daverage)/(nnn - 1);
		m_dsigma = m_dsigma > 0? sqrt(m_dsigma): 0;
	}
}

