#include "CameraCapture.h"

CameraCapture::CameraCapture()
{	
  m_camera = nullptr;
  frameGrabber = nullptr;
}

void CameraCapture::init(ImageBuffer *buffer)
{
  m_buffer = buffer;
}

void CameraCapture::start()
{

  if(nullptr != m_camera && nullptr !=frameGrabber)
  {
	m_camera->open();
	frameGrabber->open();
  }
}

void CameraCapture::stop()
{
  if(nullptr != m_camera)
  {
	frameGrabber->close();
	m_camera->close();
	
  }
  frameGrabber = nullptr;
  m_camera = nullptr;
}

void CameraCapture::newFrame(IplImage* frame)
{
  if(nullptr != m_buffer)
  {
	m_buffer->pushFrame(frame);
  }
  else
  {
	//	Log an error and drop the frame
	Logger::logError("CameraCapture - newFrame: Receiving new frames but dont have an image buffer. Just dropping them :(");
  }
}

void CameraCapture::setCamera(unique_ptr<lens::ICamera> camera)
{
  if(nullptr != camera)
  {
	m_camera = ::move(camera);

	
	unique_ptr<lens::FrameGrabber> tmpGrabber(new lens::FrameGrabber(*m_camera));
	frameGrabber = ::move(tmpGrabber);
	//m_camera->addObserver(this);
	frameGrabber ->addObserver(this);
  }
}

bool CameraCapture::hasCamera(void)
{ 
  return nullptr != m_camera;
}
