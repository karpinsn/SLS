#include "FrameCapture.h"

FrameCapture::FrameCapture(void* callbackInstance, void (*newFrameCallback)(void* callbackInstance, shared_ptr<IplImage> newFrame)) : 
QThread(), m_callbackInstance(callbackInstance), m_newFrameCallback(newFrameCallback)
{	
  m_running = false;
  //	Needed so that we can emit shared_ptr<IplImage> with newFrame
  qRegisterMetaType<shared_ptr<IplImage> >("shared_ptr<IplImage>");
}

void FrameCapture::init(ImageBuffer* buffer, QGLWidget* context)
{
  m_oglContext = unique_ptr<QGLWidget>(new QGLWidget(QGLFormat(QGL::SampleBuffers), 0, context));
   m_buffer = buffer;
   m_running = true;
}

void FrameCapture::run()
{
  m_oglContext->makeCurrent();

  while(m_running)
  {
    shared_ptr<IplImage> frame = m_buffer->popFrame();
	m_newFrameCallback(m_callbackInstance, frame);
  }
}
