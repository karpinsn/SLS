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

EncodingOpenGLWidget::~EncodingOpenGLWidget()
{
}

void EncodingOpenGLWidget::reinit(float width, float height)
{
  m_width = width;
  m_height = height;
  glInit();

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

  if(nullptr != m_glContext)
  {
    m_glContext->init(m_width, m_height);
  }

  // Set the clear color
  qglClearColor(m_clearColor);

  // Set up the rest of the rendering
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
}

void EncodingOpenGLWidget::setGLContext(IGLContext* glContext)
{
  m_glContext = glContext;
  makeCurrent();
}

void EncodingOpenGLWidget::setEncodingContext(IEncodingGLContext* encodingContext)
{
  m_encodingContext = encodingContext;
}

void EncodingOpenGLWidget::setDecodingContext(IDecodingGLContext* decodingContext)
{
  m_decodingContext = decodingContext;
}

void EncodingOpenGLWidget::updateScene()
{
  updateGL();
}

MeshInterchange* EncodingOpenGLWidget::encode()
{
	cout << m_codecLock->available() << endl;
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

  return m_decodingContext->decode();
}

void EncodingOpenGLWidget::paintGL()
{
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  if(nullptr != m_glContext)
  {
	if(m_encode)
	{
	  m_encodingContext->encode();
	}
	else if(m_decode)
	{
	  m_glContext->draw();
	}
	else
	{
	  //  Neither encode or decode, probably preview. Just draw
	  m_glContext->draw();
	}
  }

  glPopMatrix();

  //	Print any errors if we have them
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    cout << "OpenGL Error: " << gluErrorString(error) << endl;
  }
  m_codecLock->release();
}

void EncodingOpenGLWidget::resizeGL(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if(nullptr != m_glContext)
  {
    m_glContext->resize(width, height);
  }

  cout << "Width x Height: " << width << " x " << height << endl;
  glMatrixMode(GL_MODELVIEW);
}

void EncodingOpenGLWidget::mousePressEvent(QMouseEvent *event)
{
  if(nullptr != m_glContext)
  {
	m_glContext->mousePressEvent(event->pos().x(), event->pos().y());
	updateGL();
  }
}

void EncodingOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
  if(nullptr != m_glContext)
  {
	m_glContext->mouseMoveEvent(event->pos().x(), event->pos().y());
	updateGL();
  }
}

void EncodingOpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
  updateGL();
}