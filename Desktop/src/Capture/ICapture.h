/*
 Filename:		ICapture.h
 Author:		Nikolaus Karpinsky
 Date Created:	07/22/12
 Last Edited:	07/22/12
 
 Revision Log:
 07/22/12 - Nik Karpinsky - Original creation.
 */

#ifndef _ICAPTURE_H_
#define _ICAPTURE_H_

#include "IGLContext.h"

class ICapture : public IGLContext
{
public:
  virtual ~ICapture(void) { };

  virtual void resizeInput(float width, float height) = 0;
  virtual void captureReferencePlane(void)			  = 0;
  virtual void setGammaCutoff(float gammaValue)		  = 0;
  virtual void setScalingFactor(float scalingFactor)  = 0;
  virtual double getFrameRate(void)					  = 0;
  virtual double get3DRate(void)					  = 0;
  virtual bool newImage(IplImage *frame)			  = 0;
};

#endif	// _ICAPTURE_H_
