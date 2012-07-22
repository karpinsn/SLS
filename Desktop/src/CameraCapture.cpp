#include "CameraCapture.h"

CameraCapture::CameraCapture()
{	
  m_camera = nullptr;
}

CameraCapture::~CameraCapture()
{
  if(nullptr != m_camera)
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
  if(nullptr != m_camera)
  {
    m_camera->open();
  }
}

void CameraCapture::stop()
{
  if(nullptr != m_camera)
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
  if(nullptr != camera)
  {
    //  Delete the old camera if we have one
    if(nullptr != m_camera)
    {
      delete m_camera;
    }

    m_camera = camera;
    m_camera->init();
    m_camera->addObserver(this);
  }
}
