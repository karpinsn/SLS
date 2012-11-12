#include "EncodingOpenGLWidget.h"

EncodingOpenGLWidget::EncodingOpenGLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  m_clearColor = QColor::fromRgb(0, 0, 0, 0);
  m_width = 512;
  m_height = 512;
  m_encode = false;
  m_decode = false;
  m_preview = false;
  m_codecLock = shared_ptr<QSemaphore>(new QSemaphore(1));
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
  std::cout << "Using GLEW Version: " << glewGetString(GLEW_VERSION) << endl;

  //  Used for previews
  m_textureDisplay.init();

  // Set the clear color
  qglClearColor(m_clearColor);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}

void EncodingOpenGLWidget::setEncodingContext(IEncoder* encodingContext)
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
  m_preview = false;
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
  m_preview = false;
  updateGL(); //  Once this is finished it will unlock the mutex
  m_codecLock->acquire();
  m_codecLock->release();

  return new MeshInterchange(&m_decodingContext->getDepthMap(), false);
}

void EncodingOpenGLWidget::previewEncode(void)
{
  //  Just request the draw which will go to the screen
  m_encode = true;
  m_preview = true;
  updateGL();
}

void EncodingOpenGLWidget::previewDecode(void)
{
  //  Just request the draw which will go to the screen
  m_decode = true;
  m_preview = true;
  updateGL();
}

void EncodingOpenGLWidget::paintGL()
{
  if(m_encode && m_preview && nullptr != m_encodingContext)
  {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_encodingContext->encode();
	m_textureDisplay.draw(m_encodingContext->getEncodedData()->getTexture());
  }
  else if(m_decode && m_preview && nullptr != m_decodingContext)
  {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_decodingContext->decode();
	m_textureDisplay.draw(&m_decodingContext->getDepthMap());
  }
  else if(m_encode && nullptr != m_encodingContext)
  {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_encodingContext->encode();
	m_codecLock->release();
  }
  else if(m_decode && nullptr != m_decodingContext)
  {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_decodingContext->decode();
	m_codecLock->release();
  }

  // Make sure we dont have any errors
  OGLStatus::logOGLErrors("EncodingOpenGLWidget - paintGL()");
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