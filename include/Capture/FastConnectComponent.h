// FastConnectComponent.h: CFastConnectComponent class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
using namespace std;

class CFastConnectComponent 
{
public:
	CFastConnectComponent();

	void FastConnectFindStartPoint(int &startx, int &starty);
	int  FastConnectAlg( const unsigned char *wrappedMask, unsigned char *unwrappedMask,
		const int &imageWidth, const int &imageHeight,const int &background, const int&valid);

	void FastConnectBottomRight(const int &xstart, const int &ystart);
	void FastConnectTopRight(const int &xstart, const int &ystart);
	void FastConnectBottomLeft(const int &xstart, const int &ystart);
	void FastConnectTopLeft(const int &xstart, const int &ystart);

	const unsigned char *m_ucmask;
	unsigned char *m_uclocalmask;

	int  m_iwidth;
	int  m_iheight;
	int  m_valid;
	int  m_background;
	vector<int> m_ipossible;
};
