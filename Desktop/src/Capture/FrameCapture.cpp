#include "FrameCapture.h"

FrameCapture::FrameCapture() : QThread()
{	
  m_running = false;
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
    IplImage* frame = m_buffer->popFrame();

    emit(newFrame(frame));
  }
}
