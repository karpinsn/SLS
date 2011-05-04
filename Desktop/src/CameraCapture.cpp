#include "CameraCapture.h"

CameraCapture::CameraCapture() : QThread()
{	
}

void CameraCapture::init(ImageBuffer *buffer)
{
  m_capture = cvCaptureFromCAM(CV_CAP_ANY);
  m_paused = false;
  m_buffer = buffer;
}

CameraCapture::~CameraCapture()
{
  if(NULL != m_capture)
  {
    cvReleaseCapture(&m_capture);
  }
}

bool CameraCapture::isConnected()
{
  return m_capture != NULL;
}

void CameraCapture::run()
{
  while(true)
  {
    m_pauseMutex.lock();

    if(m_paused)
    {
      m_pauseCondition.wait(&m_pauseMutex);
    }

    m_pauseMutex.unlock();

    if(NULL != m_capture)
    {
      IplImage* image = cvQueryFrame(m_capture);
      m_buffer->pushFrame(image);
    }
  }
}

int CameraCapture::pause()
{
  m_pauseMutex.lock();
  m_paused = true;
  m_pauseMutex.unlock();
}

int CameraCapture::resume()
{
  m_paused = false;
  m_pauseCondition.wakeAll();
}

int CameraCapture::getWidth()
{
  int width = 0;

  if(NULL != m_capture)
  {
    width = cvGetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_WIDTH);
  }

  return width;
}

int CameraCapture::getHeight()
{
  int height = 0;

  if(NULL != m_capture)
  {
    height = cvGetCaptureProperty(m_capture, CV_CAP_PROP_FRAME_HEIGHT);
  }

  return height;
}
