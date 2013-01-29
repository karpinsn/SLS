#include "FrameCapture.h"

FrameCapture::FrameCapture(void* callbackInstance, void (*newFrameCallback)(void* callbackInstance, shared_ptr<IplImage> newFrame))
{	
  m_worker = new FrameCaptureWorker(callbackInstance, newFrameCallback);
}

void FrameCapture::init(ImageBuffer* buffer, QGLWidget* context)
{
  m_workerThread = new QThread();
  
  m_worker->moveToThread(m_workerThread);
  m_worker->init(buffer, context);

  //  Connect the thread and its timer 
  connect(m_workerThread, SIGNAL(started()),  m_worker, SLOT(grabFrames()));
  connect(m_worker, SIGNAL(finished()), m_workerThread, SLOT(quit()));
  connect(m_workerThread, SIGNAL(finished()), m_workerThread, SLOT(deleteLater()));
  connect(m_worker, SIGNAL(finished()), m_worker, SLOT(deleteLater()));  
}

void FrameCapture::start(void)
{
  m_workerThread->start();
}

FrameCaptureWorker::FrameCaptureWorker(void* callbackInstance, void (*newFrameCallback)(void* callbackInstance, shared_ptr<IplImage> newFrame)) 
  : m_callbackInstance(callbackInstance), m_newFrameCallback(newFrameCallback)
{
  m_running = false;

  //	Needed so that we can emit shared_ptr<IplImage> with newFrame
  qRegisterMetaType<shared_ptr<IplImage> >("shared_ptr<IplImage>");
}

void FrameCaptureWorker::init(ImageBuffer* buffer, QGLWidget* context)
{
  m_buffer = buffer;
  m_oglContext = unique_ptr<QGLWidget>(new QGLWidget(QGLFormat(QGL::SampleBuffers), 0, context));
  m_running = true;
}

void FrameCaptureWorker::stop(void)
{
  m_running = false;
}

void FrameCaptureWorker::grabFrames()
{
  m_oglContext->makeCurrent();

  while(m_running)
  {
	shared_ptr<IplImage> frame = m_buffer->popFrame();
	m_newFrameCallback(m_callbackInstance, frame);
  }

  emit finished();
}
