// FastConnectComponent.cpp: implementation of the CFastConnectComponent class.
//
//////////////////////////////////////////////////////////////////////

#include "Capture/FastConnectComponent.h"
#include <vector>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////
//
// Constructor
//
//////////////////////////////////////////////////////////////////////

CFastConnectComponent::CFastConnectComponent()
{
	m_iwidth = 256;
	m_iheight = 256;
	m_ucmask = nullptr;
	m_uclocalmask = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
//   
// Returns:
//
// Parameters:
//   wrappedMask == mask of wrapped phase
//   unwrappedMask == mask of unwrapped phase
//   imageWidth == width of the image
//   imageHeight == height of the image
//   background == value to represent background in the unwrapped mask
//   valid == value to represent the valid pixel of the unwrapped mask
//
//////////////////////////////////////////////////////////////////////////////////

int CFastConnectComponent::FastConnectAlg( const unsigned char *wrappedMask, unsigned char *unwrappedMask,
		const int &imageWidth, const int &imageHeight, const int &background,const int&valid)
{
	m_iwidth = imageWidth;
	m_iheight = imageHeight;
	m_ucmask = wrappedMask;
	m_uclocalmask = unwrappedMask;
	m_valid = valid & 0xff;
	m_background = background;

	// Set the background of the unwrapped mask
	memset( m_uclocalmask, m_background, m_iwidth * m_iheight * sizeof(m_uclocalmask[0]));

	// Start from the center of the image
	int istartx = m_iwidth >> 1;
	int istarty = m_iheight >> 1;

	// Find a point to search for a connect area of the wrapped mask
	FastConnectFindStartPoint(istartx, istarty);

	// Set the start point in unwrapped as valid 
	int k = istarty * m_iwidth + istartx;
	m_uclocalmask[k] = m_valid;

	// Find connected area to the bottom right of the start point
	FastConnectBottomRight(istartx, istarty);
	// Find connected area to the top right of the start point
	FastConnectTopRight(istartx, istarty);
	// Find connected area to the bottom left of the start point
	FastConnectBottomLeft(istartx, istarty);
	// Find connected area to the top left of the start point
	FastConnectTopLeft(istartx, istarty);

	// return the index of the start point
	return k ;
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description: Find a start point for connected area search
// 
// Returns:
//
// Parameters:
//   startx == the x coordinate of start point of the connected area search
//   starty == the y coordiante of start point of hte connected area search
//
//////////////////////////////////////////////////////////////////////////////////

void CFastConnectComponent::FastConnectFindStartPoint(int &startx, int &starty)
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
			if (m_ucmask[ k + i] & // current pixel
				m_ucmask[ k + i + 1] & //right neighbor
				m_ucmask[ k + i - 1] & //left point
				m_ucmask[ k + m_iwidth + i] & // bottom neighbor
				m_ucmask[ k - m_iwidth + i]&  // top neighbor
				m_ucmask[ k - m_iwidth + i + 1] & // top right
				m_ucmask[ k - m_iwidth + i - 1] & // top left
				m_ucmask[ k + m_iwidth + i + 1] & // bottom right
				m_ucmask[ k + m_iwidth + i - 1]) // bottom left

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
				if (m_ucmask[ k + i] & // current pixel
				m_ucmask[ k + i + 1] & //right neighbor
				m_ucmask[ k + i - 1] & //left point
				m_ucmask[ k + m_iwidth + i] & // bottom neighbor
				m_ucmask[ k - m_iwidth + i]&  // top neighbor
				m_ucmask[ k - m_iwidth + i + 1] & // top right
				m_ucmask[ k - m_iwidth + i - 1] & // top left
				m_ucmask[ k + m_iwidth + i + 1] & // bottom right
				m_ucmask[ k + m_iwidth + i - 1]) // bottom left
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
			if (m_ucmask[ k + i] & // current pixel
				m_ucmask[ k + i + 1] & //right neighbor
				m_ucmask[ k + i - 1] & //left point
				m_ucmask[ k + m_iwidth + i] & // bottom neighbor
				m_ucmask[ k - m_iwidth + i]&  // top neighbor
				m_ucmask[ k - m_iwidth + i + 1] & // top right
				m_ucmask[ k - m_iwidth + i - 1] & // top left
				m_ucmask[ k + m_iwidth + i + 1] & // bottom right
				m_ucmask[ k + m_iwidth + i - 1]) // bottom left
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
			if (m_ucmask[ k + i] & // current pixel
				m_ucmask[ k + i + 1] & //right neighbor
				m_ucmask[ k + i - 1] & //left point
				m_ucmask[ k + m_iwidth + i] & // bottom neighbor
				m_ucmask[ k - m_iwidth + i]&  // top neighbor
				m_ucmask[ k - m_iwidth + i + 1] & // top right
				m_ucmask[ k - m_iwidth + i - 1] & // top left
				m_ucmask[ k + m_iwidth + i + 1] & // bottom right
				m_ucmask[ k + m_iwidth + i - 1]) // bottom left
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
//		Search points connected to the start pont in the bottom top quater
//   of the image
// Returns:
//
// Parameters:
//   startx == the x coordinate of the start point
//   starty == the y coordinate of the start point
//
//////////////////////////////////////////////////////////////////////////////////

void CFastConnectComponent::FastConnectBottomRight(const int &xstart, const int &ystart)
{
	int i, j, k;
	m_ipossible.clear();
	int lineindex = ystart* m_iwidth;

	// Scan the vetical line below the start point
	for ( j = ystart + 1; j < m_iheight - 1; j++)
	{
		lineindex += m_iwidth;
		k = lineindex + xstart;
		if( m_ucmask[k] )
		{
			if( m_uclocalmask[k - m_iwidth] == m_valid )// This point has a neighbor with unwrapped phase on its top
			{
				// Set the mask
				m_uclocalmask[k] = m_valid;
			}
			//else if(m_ucmask[k + m_iwidth] || m_ucmask[k + 1] || m_ucmask[k - 1 + m_iwidth])
			// "|" is used instead of "||" to optimize in speed
			// bottom, right, and bottom left neighbor
			else if(m_ucmask[k + m_iwidth] | m_ucmask[k + 1] | 
					m_ucmask[k - 1 + m_iwidth])
			{
				m_ipossible.push_back(k);
			}
		}
	}
	
	// Scan the bottom-right part of the start point
	for(j = ystart ; j < m_iheight - 1; j++)
	{
		lineindex = j * m_iwidth;
		for( i = xstart + 1 ; i < m_iwidth -1; i++)
		{
			k =  lineindex + i;
			if( m_ucmask[k] )
			{
				if( m_uclocalmask[k - m_iwidth] == m_valid || m_uclocalmask[k - 1]== m_valid)// This point has a neighbor with unwrapped phase on its top
				{
					// Set the mask
					m_uclocalmask[k] = m_valid;
				}
				//else if(m_ucmask[k + m_iwidth] || m_ucmask[k + 1])||m_ucmask[k - 1 + m_iwidth])
				// "|" is used instead of "||" to optimize in speed
				// bottom, right, bottom left
				else if(m_ucmask[k + m_iwidth] | m_ucmask[k + 1] | 
					m_ucmask[k - 1 + m_iwidth])
				{
					m_ipossible.push_back(k);
				}
			}
		}
	}
	// Then deal with the points that have neighbor with known phase on the right, bottom or bottom left
	for (size_t ii = m_ipossible.size(); ii > 0 ; ii--)
	{
		k = m_ipossible[ii - 1];
		m_uclocalmask[k] = ( m_uclocalmask[k + 1]== m_valid || 
			 m_uclocalmask[k + m_iwidth] == m_valid || m_uclocalmask[k + m_iwidth - 1] == m_valid)?m_valid:m_background;
	} 
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
//		Search all the points connected to the start point in the top right quater
//	in the image.
// Returns:
//
// Parameters:
//   startx == the x coordinate of the start point
//   starty == the y coordinate of the start point 
//
//////////////////////////////////////////////////////////////////////////////////

void CFastConnectComponent::FastConnectTopRight(const int &xstart, const int &ystart)
{
	int i, j, k;
	m_ipossible.clear();

	// Scan Top-right part to the start point
	for(j = ystart -1; j > 1; j--)
	{
		int lineindex = j * m_iwidth;
		for( i = xstart ; i < m_iwidth -1; i++)
		{
			k = lineindex + i;
			if( m_ucmask[k])
			{
				if( m_uclocalmask[k + m_iwidth] == m_valid || m_uclocalmask[k - 1] == m_valid)// This point has a neighbor with unwrapped phase at its bottom
				{
					// Set the mask
					m_uclocalmask[k] = m_valid;
				}
				// "|" is used instead of "||" to optimize in speed
				// top, right, top left
				else if(m_ucmask[k - m_iwidth] | m_ucmask[k + 1] |
					m_ucmask[k - 1 - m_iwidth])
				{
					m_ipossible.push_back(k);
				}
			}
		}
	}
	// Then deal with the points that have neighbor with known phase on the top, right, or top left
	for (size_t ii = m_ipossible.size(); ii > 0 ; ii--)
	{
		k = m_ipossible[ii - 1];
		m_uclocalmask[k] = ( m_uclocalmask[k + 1]== m_valid || m_uclocalmask[k - m_iwidth] == m_valid ||
			m_uclocalmask[k - 1 - m_iwidth] == m_valid)?m_valid:m_background;
	} 
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
//		Search all the points connected to the start point in the bottom left quater
//	 of the image
// Returns:
//
// Parameters:
//   startx == the x coordinate of the start point
//   starty == the y coordinate of the start point
//
//////////////////////////////////////////////////////////////////////////////////

void CFastConnectComponent::FastConnectBottomLeft(const int &xstart, const int &ystart)
{
	int i, j, k;
	m_ipossible.clear();

	// Scan the Bottom Left part to the start point
	for(j = ystart ; j < m_iheight - 1; j++)
	{
		int lineindex = j * m_iwidth;
		for( i = xstart - 1 ; i > 1; i--)
		{
			k = lineindex + i;
			if( m_ucmask[k])
			{
				if( m_uclocalmask[k - m_iwidth]== m_valid || m_uclocalmask[k + 1] == m_valid)// This point has a neighbor with unwrapped phase at its top
				{
					// Set the mask
					m_uclocalmask[k] = m_valid;
				}
				//else if(m_ucmask[k + m_iwidth] | m_ucmask[k - 1])
				// bottom, left, bottom right
				else if(m_ucmask[k + m_iwidth] | m_ucmask[k - 1] | 
					m_ucmask[k + m_iwidth + 1]) // considering 6 neighbors 
				{
					m_ipossible.push_back(k);
				}
			}
		}
	}
	// Then deal with the points that have neighbor with known phase on the right or top
	for (size_t ii = m_ipossible.size(); ii > 0 ; ii--)
	{
		k = m_ipossible[ii - 1];
		m_uclocalmask[k] = ( m_uclocalmask[k -1] == m_valid || m_uclocalmask[k + m_iwidth] == m_valid ||
			m_uclocalmask[k + m_iwidth + 1] == m_valid)?m_valid:m_background;
	} 
}

//////////////////////////////////////////////////////////////////////////////////
//
// Description:
//		Serach all points connected to the start point in the top left quater
//	of the image
// Returns:
//
// Parameters:
//   startx ==
//   starty ==
//
//////////////////////////////////////////////////////////////////////////////////

void CFastConnectComponent::FastConnectTopLeft(const int &xstart, const int &ystart)
{
	int i,j,k,m;
	m_ipossible.clear();

	// Scan the Top-Left part to the start point
	for(j = ystart -1; j > 1; j--)
	{
		int lineindex = j * m_iwidth;
		for( i = xstart - 1 ; i > 1; i--)
		{
			k = lineindex + i;
			if( m_ucmask[k])
			{
				m = k + m_iwidth;
				if(m_uclocalmask[m] == m_valid || m_uclocalmask[k + 1] == m_valid)// This point has a neighbor with unwrapped phase at its bottom
				{
					// Set the mask
					m_uclocalmask[k] = m_valid;
					continue;
				}
				// top, left, top right
				else if(m_ucmask[k - m_iwidth] | m_ucmask[k - 1] |
					m_ucmask[k- m_iwidth + 1])
				{
					m_ipossible.push_back(k);
				}
			}
		}
	}
	// Then deal with the points that have neighbor with known phase on the top, left, or top right
	for (size_t ii = m_ipossible.size(); ii > 0 ; ii--)
	{
		k = m_ipossible[i - 1];
		m_uclocalmask[k] = ( m_uclocalmask[k - 1] == m_valid || m_uclocalmask[k - m_iwidth] == m_valid ||
			m_uclocalmask[k - m_iwidth + 1] == m_valid)?m_valid:m_background;
	} 
}
