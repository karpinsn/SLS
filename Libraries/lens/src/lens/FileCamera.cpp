/*
 *  FileCamera.cpp
 *
 *  Created by Nikolaus Karpinsky on 9/20/10.
 *  Copyright 2010 ISU. All rights reserved.
 *
 */

#include "FileCamera.h"

lens::FileCamera::FileCamera() : QThread()
{
  m_capture = NULL;
}

lens::FileCamera::~FileCamera()
{
  if(NULL != m_capture)
  {
    cvReleaseCapture(&m_capture);
  }
}

void lens::FileCamera::init(void)
{
  QString file = QFileDialog::getOpenFileName(NULL, "Select a multiwavelength movie to open", "/", "Movies (*.avi)");

  //	If we dont have a currently selected item then selected the first in the list
  if (NULL != file && !file.isEmpty())
  {
    m_currentFileName = file.toStdString();
    m_capture = cvCaptureFromAVI(m_currentFileName.c_str());
  }
}

void lens::FileCamera::open(void)
{
  m_running = true;
  this->start();
}

void lens::FileCamera::close(void)
{
  m_running = false;
}

float lens::FileCamera::getWidth(void)
{
  return cvGetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_WIDTH);
}

float lens::FileCamera::getHeight(void)
{
  return cvGetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_HEIGHT);
}

std::string lens::FileCamera::cameraName(void)
{
  return "File Based Camera Driver";
}

void lens::FileCamera::run()
{
  while(m_running)
  {
    if(NULL != m_capture)
    {
      IplImage* image = cvQueryFrame(m_capture);
      if(NULL != image)
      {
        notifyObservers(image);
      }
      else
      {
        cvReleaseCapture(&m_capture);
        m_capture = cvCaptureFromAVI(m_currentFileName.c_str());
      }
    }
    msleep(100.0);
  }
}
