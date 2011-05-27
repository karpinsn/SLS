/*
 Filename:		Camera.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 */

#ifndef _LENS_CAMERA_H_
#define _LENS_CAMERA_H_

#include <string>
#include <vector>
#include <cv.h>
#include <highgui.h>

#include "CameraObserver.h"

namespace lens
{
  class Camera
  {
  private:
    std::vector<CameraObserver*> m_observers;

  public:
    virtual void        init(void) = 0;
    virtual void        open(void) = 0;
    virtual void        close(void) = 0;
    virtual float       getWidth(void) = 0;
    virtual float       getHeight(void) = 0;

    void addObserver(CameraObserver *observer);

  protected:
    void notifyObservers(IplImage *frame);
  };
}

#endif	// _LENS_CAMERA_H_
