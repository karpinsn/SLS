#include "FrameCapture.h"

FrameCapture::FrameCapture() : QThread()
{	
  m_running = false;
  //	Needed so that we can emit shared_ptr<IplImage> with newFrame
  qRegisterMetaType<shared_ptr<IplImage> >("shared_ptr<IplImage>");
}

FrameCapture::~FrameCapture()
{

}

void FrameCapture::init(ImageBuffer* buffer)
{
   m_buffer = buffer;
   m_running = true;
}

void FrameCapture::run()
{
  while(m_running)
  {
    shared_ptr<IplImage> frame = m_buffer->popFrame();

    emit(newFrame(frame));
  }
}
