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

#include <cv.h>

#include "IGLContext.h"
#include "SaveStream.h"

class ICapture : public IGLContext
{
public:
  virtual ~ICapture(void) { };

  enum DisplayMode 
  {
	Geometry,
	Phase,
	Depth
  };

  virtual int getWidth() = 0;
  virtual int getHeight() = 0;
  virtual void resizeInput(int width, int height) = 0;
  virtual void loadReferencePlane(void* callbackInstance, shared_ptr<IplImage> (*imageLoaderFunction)(void* callbackInstance)) = 0;
  virtual void captureReferencePlane(void)			  = 0;
  virtual void setDisplayMode(enum DisplayMode mode)  = 0;
  virtual void setGammaCutoff(float gammaValue)		  = 0;
  virtual void setBlackLevel(float blackLevel)		  = 0;
  virtual void setScalingFactor(float scalingFactor)  = 0;
  virtual void setShiftFactor(float shiftFactor)	  = 0;
  virtual void setSaveStream(shared_ptr<SaveStream> saveStream) = 0;
  virtual double getFrameRate(void)					  = 0;
  virtual double get3DRate(void)					  = 0;
  virtual bool newImage(IplImage *frame)			  = 0;
};

#endif	// _ICAPTURE_H_
