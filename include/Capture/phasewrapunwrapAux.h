// PhaseWrapunwrapAux.h : Constants and functions for phase wrap and unwrap
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cmath>

#define POS_RES			0x01 // 1st bit
#define NEG_RES			0x02 // 2nd bit
#define VISITED			0x04 // 3rd bit
#define ACTIVE			0x08 // 4th bit
#define BRANCH_CUT		0x10 // 5th bit
#define BORDER			0x20 // 6th bit
#define UNWRAPPED		0x40 // 7th bit
#define POSTPONED		0x80 // 8th bit
#define RESIDUE			(POS_RES | NEG_RES)
#define AVOID			(BRANCH_CUT | BORDER)


// exit code
#define BAD_USAGE					10
#define MEMORY_ALLOCATION_ERROR		20
#define FILE_OPEN_ERROR				30
#define FILE_READ_ERROR				31
#define FILE_WRITE_ERROR			32
#define FILE_CLOSE_ERROR			33
#define BAD_PARAMETER				100
#define BAD_DIMENSION				110

typedef enum _PhaseWrapMethod
{
	Normal3Steps = 0,
	TrapezoidalLUT,
	fastThreeStep120,
	fourStep, 
	generalAlg,
	TwoPlusOneAlg
}PhaseWrapMethod;

typedef enum _Phase2CoordMethod
{
	absoluteXYZ = 0, 
	referencePlane, 
	removeSlope
}Phase2CoordMethod;

typedef  enum _PhaseUnwrapMode
{ 
	FastUnwrap = 0,
	ThresholdedFast,
	variance, 
	gradient, 
	pseudocorr, 
	corr_coeffs, 
	dxdygrad, 
}PhaseUnwrapMethod;

const double _double2fixmagic = 6755399441055744.0;//68719476736.0*1.5;     //2^36 * 1.5,  (52-_shiftamt=36) uses limited precisicion to floor
// const int32 _shiftamt        = 16;                    //16.16 fixed point representation,

static const double half_pi = 1.57079632679489661923;
static const double pi = 3.14159265358979323846;
static const double two_pi = 2.0*3.14159265358979323846;
static const double four_pi = 4.0*3.14159265358979323846;
static const double one_over_two_pi = 0.318309886183790671538*0.5;

static const double sqrt_3 = sqrt(3.0);
static const float half = 0.5f;
static const double one = 1.0;

static const float _CV_ATAN_CF0 = (-15.8131890796f/360.0f);
static const float _CV_ATAN_CF1 = (61.0941945596f/360.0f);
static const float _CV_ATAN_CF2 = 0.f; /*(-0.140500406322f)*/

#if BigEndian_
	#define iexp_				0
	#define iman_				1
#else
	#define iexp_				1
	#define iman_				0
#endif //BigEndian_		

// ================================================================================================
// Real2Int
// ================================================================================================

inline int Real2Int(double val)
{

#if DEFAULT_CONVERSION
	return val;
#else
	val	= val + _double2fixmagic;
	return ((int*)&val)[iman_];
#endif
}

// 0.5f is add to the end for bias
static const float icvAtanTab[8] = 
	{	0.f + _CV_ATAN_CF2  + 0.5f, 0.25f - _CV_ATAN_CF2 + 0.5f,
		0.5f - _CV_ATAN_CF2 + 0.5f, 0.25f + _CV_ATAN_CF2 + 0.5f,
		1.0f - _CV_ATAN_CF2 + 0.5f, 0.75f + _CV_ATAN_CF2 + 0.5f,
		0.5f + _CV_ATAN_CF2 + 0.5f, 0.75f - _CV_ATAN_CF2 + 0.5f
	};

static const int icvAtanSign[8] =
    { 0, 0x80000000, 0x80000000, 0, 0x80000000, 0, 0, 0x80000000 };

///////////////////////////////////////////////////////////////////////////
// Function to calculate arctangent
// The idea is from OpenCV
// I rewrote it to get a arctangent in rad
// The real arctangent should be the result minus 0.5
///////////////////////////////////////////////////////////////////////////

inline float cvFastArctan( float y, float x )
{
    int ix = *((int *) &x);
	int iy = *((int *) &y);
    int ygx, idx = ((ix < 0) << 1) + ((iy < 0) << 2 );

    ix &= 0x7fffffff;
    iy &= 0x7fffffff;

	// If iy <= ix, ygx = 0, else ygx = -1;
    ygx = (iy <= ix) - 1;
    idx -= ygx;

	// idx &= ((ix == 0) - 1) | ((iy == 0) - 1);
	// If one of ix or iy is equal to zero,
	// Then set idx to zero,
	// else, keep the value in idx.
	idx &= (!((ix | iy) ^ 0)) - 1;

    // swap ix and iy if ix < iy
    ix ^= iy & ygx;
    iy ^= ix & ygx;
    ix ^= iy & ygx;

    iy ^= icvAtanSign[idx];

    // set ix to non zero
    ix |= 1;
    {
        float z = *((float *) &iy) / *((float *) &ix);
        return ((_CV_ATAN_CF0 * fabs(z) + _CV_ATAN_CF1 ) * z + icvAtanTab[idx]);
    }
}