/*
 Filename:		FileCamera.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10

 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 */

#ifndef _FILE_CAMERA_H_
#define _FILE_CAMERA_H_

#include <QThread>
#include <QFileDialog>

#include <cv.h>
#include <highgui.h>

#include "Camera.h"

using namespace std;

namespace lens
{
    class FileCamera : public Camera, QThread
        {
    private:
      CvCapture   *m_capture;
      bool        m_running;

      string      m_currentFileName;

    public:
      FileCamera();
      ~FileCamera();

      virtual void  init(void);
      virtual void  open(void);
      virtual void  close(void);
      virtual float getWidth(void);
      virtual float getHeight(void);

      static std::string cameraName(void);

    protected:
      void run();
    };
}

#endif	// _FILE_CAMERA_H_
