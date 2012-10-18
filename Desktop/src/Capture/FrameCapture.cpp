#include "FrameCapture.h"

FrameCapture::FrameCapture(void* callbackInstance, void (*newFrameTest)(void* callbackInstance, shared_ptr<IplImage> newFrame)) : 
QThread(), m_callbackInstance(callbackInstance), m_newFrameTest(newFrameTest)
{	
  m_running = false;
  //	Needed so that we can emit shared_ptr<IplImage> with newFrame
  qRegisterMetaType<shared_ptr<IplImage> >("shared_ptr<IplImage>");
}

FrameCapture::~FrameCapture()
{

}

void FrameCapture::init(ImageBuffer* buffer, OpenGLWidget* context)
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
    //emit(newFrame(frame));
	m_newFrameTest(m_callbackInstance, frame);
	//	Give a little sleep time so as not to overwhelm the thread
	msleep(1000/6);
  }
}
