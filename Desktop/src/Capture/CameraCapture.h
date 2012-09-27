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

#include <wrench/Logger.h>

#include <lens/Camera.h>
#include <lens/CameraObserver.h>
#include <lens/OpenCVCamera.h>

#include "ImageBuffer.h"

using namespace wrench;

class CameraCapture : public lens::CameraObserver
{
private:
  unique_ptr<lens::Camera> m_camera;
  ImageBuffer   *m_buffer;

public:
    CameraCapture();
    ~CameraCapture();

    void init(ImageBuffer *buffer);
    void start();
    void stop();
    virtual void newFrame(IplImage* frame);

	/**
	* Sets the camera to be used by this CameraCapture. 
	*
	* Sets the camera to be used by this CameraCapture. CameraCapture takes exclusive
	* ownership of the camera, thus move must be used in the function call to
	* transfer ownership (vs copy)
	*
	* @param camera Camera to use for capture.
	*/
    void setCamera(unique_ptr<lens::Camera> camera);
	
	/**
	* Returns true or false of whether or not the CameraCapture has a camera.
	*
	* @return True or false of whether or not the CameraCapture has a camera.
	*/
	bool hasCamera(void);
 };

#endif	// _CAMERA_CAPTURE_H_
