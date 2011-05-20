/*
 *  OpenCVCamera.cpp
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "OpenCVCamera.h"

lens::OpenCVCamera::OpenCVCamera() : QThread()
{
  m_capture = NULL;
}

lens::OpenCVCamera::~OpenCVCamera()
{
  if(NULL != m_capture)
  {
    cvReleaseCapture(&m_capture);
  }
}

void lens::OpenCVCamera::init(void)
{
  m_capture = cvCaptureFromCAM(CV_CAP_ANY);
}

void lens::OpenCVCamera::open(void)
{
  m_running = true;
  this->start();
}

void lens::OpenCVCamera::close(void)
{
  m_running = false;
}

std::string lens::OpenCVCamera::cameraName(void)
{
	return "OpenCV Camera Driver";
}

void lens::OpenCVCamera::run()
{
  while(m_running)
  {
    if(NULL != m_capture)
    {
      IplImage* image = cvQueryFrame(m_capture);
      notifyObservers(image);
    }
  }
}
