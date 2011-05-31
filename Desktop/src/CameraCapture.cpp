#include "CameraCapture.h"

CameraCapture::CameraCapture()
{	
  m_camera = NULL;
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
  m_buffer = buffer;
}

void CameraCapture::start()
{
  if(NULL != m_camera)
  {
    m_camera->open();
  }
}

void CameraCapture::stop()
{
  if(NULL != m_camera)
  {
    m_camera->close();
  }
}

void CameraCapture::newFrame(IplImage* frame)
{
  m_buffer->pushFrame(frame);
}

void CameraCapture::setCamera(lens::Camera* camera)
{
  if(NULL != camera)
  {
    //  Delete the old camera if we have one
    if(NULL != m_camera)
    {
      delete m_camera;
    }

    m_camera = camera;
    m_camera->init();
    m_camera->addObserver(this);
  }
}
