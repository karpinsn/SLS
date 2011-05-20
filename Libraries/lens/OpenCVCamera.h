/*
 Filename:		OpenCVCamera.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 */

#ifndef _OPEN_CV_CAMERA_H_
#define _OPEN_CV_CAMERA_H_

#include <cv.h>
#include <highgui.h>
#include <QThread>

#include "Camera.h"

using namespace std;

namespace lens
{
    class OpenCVCamera : public Camera, QThread
	{
    private:
      CvCapture   *m_capture;
      bool        m_running;
    public:
      OpenCVCamera();
      ~OpenCVCamera();

      virtual void init(void);
      virtual void open(void);
      virtual void close(void);
	  virtual std::string cameraName(void);

    protected:
      void run();
	};
}

#endif	// _OPEN_CV_CAMERA_H_
