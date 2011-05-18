#include "CameraCapture.h"

CameraCapture::CameraCapture()
{	
  m_camera = new lens::OpenCVCamera();
}

CameraCapture::~CameraCapture()
{
  if(NULL != m_camera)
  {
    delete m_camera;
  }
}

void CameraCapture::init(ImageBuffer *buffer)
{
  m_camera->init();
  m_camera->addObserver(this);
  m_buffer = buffer;
}

void CameraCapture::start()
{
  m_camera->open();
}

void CameraCapture::newFrame(IplImage* frame)
{
  m_buffer->pushFrame(frame);
}
