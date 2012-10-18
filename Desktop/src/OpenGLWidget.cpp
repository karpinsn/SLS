#include "OpenGLWidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  m_clearColor = QColor::fromRgb(0, 0, 0, 0);
  m_glContext = nullptr;
  m_width = 0;
  m_height = 0;
}

OpenGLWidget::OpenGLWidget(QWidget *parent, IGLContext* glContext, QColor clearColor) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  m_glContext = glContext;
  m_clearColor = clearColor;
}

OpenGLWidget::~OpenGLWidget()
{
}

void OpenGLWidget::initializeGL()
{	
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    cout << "Failed to init GLEW: " << glewGetErrorString(err) << endl;
  }
  std::cout << "Using GLEW Version: " << glewGetString(GLEW_VERSION) << endl;

  m_glContext->init();

  // Set the clear color
  qglClearColor(m_clearColor);

  // Set up the rest of the rendering
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
}

void OpenGLWidget::setGLContext(IGLContext* glContext)
{
  m_glContext = glContext;
  makeCurrent();
}

void OpenGLWidget::updateScene()
{
  updateGL();
}

void OpenGLWidget::paintGL()
{
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  if(nullptr != m_glContext)
  {
	m_glContext->draw();
  }

  glPopMatrix();

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("OpenGLWidget - paintGL()");
}

void OpenGLWidget::resizeGL(int width, int height)
{
  m_width = width;
  m_height = height;

  if(nullptr != m_glContext)
  {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	m_glContext->resize(width, height);

	glMatrixMode(GL_MODELVIEW);
  }
}

void OpenGLWidget::cameraSelectMode(int mode)
{
  if(nullptr != m_glContext)
  {
	m_glContext->cameraSelectMode(mode);
  }
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
  //std::cout << "Mouse Pressed: (" << event->pos().x() << "," << event->pos().y() << ")" << std::endl;
  
  if(nullptr != m_glContext)
  {
	//	Perform the Y flip
	int mouseY = (m_height - 1) - event->pos().y();

	m_glContext->mousePressEvent(event->pos().x(), mouseY);
	updateGL();
  }
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
  //std::cout << "Mouse Moved: (" << event->pos().x() << "," << event->pos().y() << ")" << std::endl;

  if(nullptr != m_glContext)
  {
	int mouseY = (m_height - 1) - event->pos().y();

	m_glContext->mouseMoveEvent(event->pos().x(), mouseY);
	updateGL();
  }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent* event)
{
  //std::cout << "Mouse Released: (" << event->pos().x() << "," << event->pos().y() << ")" << std::endl;
  
  if(nullptr != m_glContext)
  {
	updateGL();
  }
}
