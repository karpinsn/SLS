/*
 Filename:		CameraCapture.h
 Author:		Nikolaus Karpinsky
 Date Created:	03/23/11
 Last Edited:	03/23/11
 
 Revision Log:
 03/23/11 - Nik Karpinsky - Original creation.
*/

#ifndef _CAMERA_CAPTURE_H_
#define _CAMERA_CAPTURE_H_

#include <cv.h>
#include <highgui.h>

#include <lens/Camera.h>
#include <lens/CameraObserver.h>
#include <lens/OpenCVCamera.h>

#include "ImageBuffer.h"

class CameraCapture : public lens::CameraObserver
{
private:
  lens::Camera  *m_camera;
  ImageBuffer   *m_buffer;

public:
    CameraCapture();
    ~CameraCapture();

    void init(ImageBuffer *buffer);
    void start();
    void stop();
    virtual void newFrame(IplImage* frame);
    void setCamera(lens::Camera* camera);
 };

#endif	// _CAMERA_CAPTURE_H_
