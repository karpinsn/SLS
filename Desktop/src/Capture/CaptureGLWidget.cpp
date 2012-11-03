#include "CaptureGLWidget.h"

CaptureGLWidget::CaptureGLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  m_clearColor = QColor::fromRgb(0, 0, 0, 0);
  m_captureDecoder = nullptr;
  m_width = 0;
  m_height = 0;
  m_displayMode = Geometry;
}

void CaptureGLWidget::initializeGL()
{	
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
	cout << "Failed to init GLEW: " << glewGetErrorString(err) << endl;
  }
  std::cout << "Using GLEW Version: " << glewGetString(GLEW_VERSION) << endl;

  m_captureDecoder->init();

  m_axis.init();
  m_controller.init(0.0f, 0.0f, 0.0f, .5f);
  m_camera.init(.5f, .5f, 2.5f, .5f, .5f, 0.0f, 0.0f, -1.0f, 0.0f);
  m_camera.setMode(1);

  m_mesh = shared_ptr<TriMesh>(new TriMesh(256, 256));
  m_mesh->initMesh();

  m_finalRenderColor.init();
  m_finalRenderColor.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/FinalRenderColor.vert"));
  m_finalRenderColor.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/FinalRenderColor.frag"));
  m_finalRenderColor.bindAttributeLocation("vert", 0);
  m_finalRenderColor.bindAttributeLocation("vertTexCoord", 1);

  m_finalRenderColor.link();
  m_finalRenderColor.uniform("normals", 0);
  m_finalRenderColor.uniform("depthMap", 1);

  m_textureDisplay.init();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  m_finalRenderColor.uniform("lightPosition", glm::vec3(0.5f, 0.5f, 4.0f));
  m_finalRenderColor.uniform("ambientColor", glm::vec4(.1, .1, .1, 1.0));
  m_finalRenderColor.uniform("diffuseColor", glm::vec4(.9, .9, .9, 1.0));
  m_finalRenderColor.uniform("specularColor", glm::vec4(1.0, 1.0, 1.0, 1.0));

  // Set the clear color
  qglClearColor(m_clearColor);

  // Set up the rest of the rendering
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
}

void CaptureGLWidget::resizeCapture(int width, int height)
{
  //  Resize our mesh that is used for display
  m_mesh = shared_ptr<TriMesh>(new TriMesh(width, height));
  m_mesh->initMesh();

  //  Now resize our capture decoder
  m_captureDecoder->resizeInput(width, height);
}

void CaptureGLWidget::setCaptureContext(ICapture* glContext)
{
  m_captureDecoder = glContext;
  makeCurrent();
}

void CaptureGLWidget::updateScene()
{
  updateGL();
}

void CaptureGLWidget::paintGL()
{
  if(nullptr != m_captureDecoder)
  {
	m_captureDecoder->draw();

	if(Geometry == m_displayMode)
	{
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  //  Get our matricies
	  glm::mat4 modelViewMatrix = glm::mat4() * m_camera.getMatrix() * m_controller.getTransform();
	  glm::mat4 projectionMatrix = glm::perspective(45.0f, 1.0f, .1f, 10.0f);
	  glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

	  m_axis.draw(modelViewMatrix);

	  m_finalRenderColor.bind();
	  {
		m_finalRenderColor.uniform("modelViewMatrix", modelViewMatrix);
		m_finalRenderColor.uniform("projectionMatrix", projectionMatrix);
		m_finalRenderColor.uniform("normalMatrix", normalMatrix);

		m_captureDecoder->getNormalMap().bind(GL_TEXTURE0);
		m_captureDecoder->getDepthMap().bind(GL_TEXTURE1);

		// Draw a plane of pixels
		m_mesh->draw();
	  }
	  m_finalRenderColor.unbind();
	}
	else if(Depth == m_displayMode)
	{
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  m_textureDisplay.draw(&m_captureDecoder->getDepthMap());
	}
  }
  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("OpenGLWidget - paintGL()");
}

void CaptureGLWidget::resizeGL(int width, int height)
{
  m_width = width;
  m_height = height;
  m_camera.reshape(width, height);
  glViewport(0, 0, width, height);
}

void CaptureGLWidget::cameraSelectMode(int mode)
{
  m_camera.setMode(mode);
}

void CaptureGLWidget::setDisplayMode(enum DisplayMode mode)
{
  m_displayMode = mode;
}

void CaptureGLWidget::mousePressEvent(QMouseEvent *event)
{
  // Perform the Y flip
  int mouseY = (m_height - 1) - event->pos().y();
  m_camera.mousePressed(event->pos().x(), mouseY);
  updateGL();
}

void CaptureGLWidget::mouseMoveEvent(QMouseEvent *event)
{
  // Perform the Y flip
  int mouseY = (m_height - 1) - event->pos().y();
  m_camera.mouseMotion(event->pos().x(), mouseY);
  updateGL();
}
