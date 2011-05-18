/*
 Filename:		CameraObserver.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 */

#ifndef _CAMERA_OBSERVER_H_
#define _CAMERA_OBSERVER_H_

#include <cv.h>
#include <highgui.h>

namespace lens
{
  class CameraObserver
  {
  public:
    virtual void newFrame(IplImage* frame) = 0;
  };
}

#endif	// _CAMERA_OBSERVER_H_
