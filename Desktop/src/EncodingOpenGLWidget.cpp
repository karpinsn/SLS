#include "EncodingOpenGLWidget.h"

EncodingOpenGLWidget::EncodingOpenGLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  m_clearColor = QColor::fromRgb(0, 0, 0, 0);
  m_glContext = nullptr;
  m_width = 512;
  m_height = 512;
  m_encode = false;
  m_decode = false;
  m_codecLock = shared_ptr<QSemaphore>(new QSemaphore(1));
}

EncodingOpenGLWidget::EncodingOpenGLWidget(QWidget *parent, IGLContext* glContext, QColor clearColor) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  m_glContext = glContext;
  m_clearColor = clearColor;
}

void EncodingOpenGLWidget::reinit(float width, float height)
{
  m_width = width;
  m_height = height;
  
  resizeGL(width, height);
}

void EncodingOpenGLWidget::initializeGL()
{	
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    cout << "Failed to init GLEW: " << glewGetErrorString(err) << endl;
  }

  // Set the clear color
  qglClearColor(m_clearColor);
}

void EncodingOpenGLWidget::setEncodingContext(IEncodingGLContext* encodingContext)
{
  m_encodingContext = encodingContext;
}

void EncodingOpenGLWidget::setDecodingContext(IDecoder* decodingContext)
{
  m_decodingContext = decodingContext;
}

void EncodingOpenGLWidget::updateScene()
{
  updateGL();
}

MeshInterchange* EncodingOpenGLWidget::encode()
{
  //  Lock the mutex
  m_codecLock->acquire();
  m_encode = true;
  updateGL(); //  Once this is finished it will unlock the mutex
  m_codecLock->acquire();
  m_codecLock->release();

  return m_encodingContext->getEncodedData();
}

MeshInterchange* EncodingOpenGLWidget::decode()
{
  //  Lock the mutex
  m_codecLock->acquire();
  m_decode = true;
  updateGL(); //  Once this is finished it will unlock the mutex
  m_codecLock->acquire();
  m_codecLock->release();

  return new MeshInterchange(&m_decodingContext->getDepthMap(), false);
}

void EncodingOpenGLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(m_encode && nullptr != m_encodingContext)
  {
	m_encodingContext->encode();
  }
  else if(m_decode && nullptr != m_decodingContext)
  {
	m_decodingContext->decode();
  }
  else
  {
	//  Neither encode or decode, probably preview. Just draw
	//m_glContext->draw();
	//  TODO - Come and fix preview
  }

  // Make sure we dont have any errors
  OGLStatus::logOGLErrors("EncodingOpenGLWidget - paintGL()");
  m_codecLock->release();
}

void EncodingOpenGLWidget::resizeGL(int width, int height)
{
  glViewport(0, 0, width, height);
}

void EncodingOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
  updateGL();
}

void EncodingOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
  updateGL();
}