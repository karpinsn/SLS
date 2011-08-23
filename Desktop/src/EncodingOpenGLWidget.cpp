#include "EncodingOpenGLWidget.h"

EncodingOpenGLWidget::EncodingOpenGLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  m_clearColor = QColor::fromRgb(0, 0, 0, 0);
  m_glContext = NULL;
  m_width = 512;
  m_height = 512;
}

EncodingOpenGLWidget::EncodingOpenGLWidget(QWidget *parent, AbstractGLContext* glContext, QColor clearColor) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
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
}

void EncodingOpenGLWidget::initializeGL()
{	
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    cout << "Failed to init GLEW: " << glewGetErrorString(err) << endl;
  }
  std::cout << "Using GLEW Version: " << glewGetString(GLEW_VERSION) << endl;

  if(NULL != m_glContext)
  {
    m_glContext->init(m_width, m_height);
  }

  // Set the clear color
  qglClearColor(m_clearColor);

  // Set up the rest of the rendering
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
}

void EncodingOpenGLWidget::setGLContext(AbstractGLContext* glContext)
{
  m_glContext = glContext;
  makeCurrent();
}

void EncodingOpenGLWidget::updateScene()
{
  updateGL();
}

void EncodingOpenGLWidget::encode()
{
  //  Lock the mutex
  m_encodingLock.acquire();
  updateGL(); //  Once this is finished it will unlock the mutex
  m_encodingLock.acquire();
  m_encodingLock.release();
}

void EncodingOpenGLWidget::paintGL()
{
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  if(NULL != m_glContext)
  {
    m_glContext->draw();
  }

  glPopMatrix();

  //	Print any errors if we have them
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    cout << "OpenGL Error: " << gluErrorString(error) << endl;
  }
  m_encodingLock.release();
}

void EncodingOpenGLWidget::resizeGL(int width, int height)
{
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if(NULL != m_glContext)
  {
    m_glContext->resize(width, height);
  }

  glMatrixMode(GL_MODELVIEW);
}
