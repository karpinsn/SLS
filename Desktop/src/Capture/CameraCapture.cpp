#include "CameraCapture.h"

CameraCapture::CameraCapture()
{	
  m_camera = nullptr;
}

CameraCapture::~CameraCapture()
{ }

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

  m_camera = nullptr;
}

void CameraCapture::newFrame(IplImage* frame)
{
  if(nullptr != m_camera)
  {
	m_buffer->pushFrame(frame);
  }
  else
  {
	//	Log an error and drop the frame
	Logger::logError("CameraCapture - newFrame: Receiving new frames but dont have an image buffer. Just dropping them :(");
  }
}

void CameraCapture::setCamera(unique_ptr<lens::Camera> camera)
{
  if(nullptr != camera)
  {
    m_camera = ::move(camera);
    m_camera->addObserver(this);
  }
}

bool CameraCapture::hasCamera(void)
{
  return nullptr != m_camera;
}
