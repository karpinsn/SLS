#include "CaptureGLWidget.h"

CaptureGLWidget::CaptureGLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
  m_clearColor = QColor::fromRgb(0, 0, 0, 0);
  m_captureDecoder = nullptr;
  m_width = 0;
  m_height = 0;
  m_displayMode = Geometry;
  
  //  This is needed so that we can trigger an update from another thread
  connect(this,	SIGNAL(crossThreadGLUpdate()), this, SLOT(updateGL())); 
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
  m_finalRenderColor.uniform("depthMap", 0);
  m_finalRenderColor.uniform("normals", 1);

  m_finalRenderTexture.init();
  m_finalRenderTexture.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/FinalRenderTexture.vert"));
  m_finalRenderTexture.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/FinalRenderTexture.frag"));
  m_finalRenderTexture.bindAttributeLocation("vert", 0);
  m_finalRenderTexture.bindAttributeLocation("vertTexCoord", 1);

  m_finalRenderTexture.link();
  m_finalRenderTexture.uniform("depthMap", 0);
  m_finalRenderTexture.uniform("normals", 1);
  m_finalRenderTexture.uniform("textureMap", 2);

  m_textureDisplay.init();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  m_finalRenderColor.uniform("lightPosition", glm::vec3(0.5f, 0.5f, 4.0f));
  m_finalRenderColor.uniform("ambientColor", glm::vec4(.1, .1, .1, 1.0));
  m_finalRenderColor.uniform("diffuseColor", glm::vec4(.9, .9, .9, 1.0));
  m_finalRenderColor.uniform("specularColor", glm::vec4(1.0, 1.0, 1.0, 1.0));

  m_finalRenderTexture.uniform("lightPosition", glm::vec3(0.5f, 0.5f, 4.0f));
  m_finalRenderTexture.uniform("diffuseColor", glm::vec4(.1, .1, .1, 1.0));
  m_finalRenderTexture.uniform("specularColor", glm::vec4(.0, .0, .0, 1.0));

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
}

void CaptureGLWidget::paintGL()
{
  if(nullptr != m_captureDecoder)
  {
	m_captureDecoder->decode();

	if(Geometry == m_displayMode || GeometryTexture == m_displayMode)
	{
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	  //  Get our matricies
	  glm::mat4 modelViewMatrix = glm::mat4() * m_camera.getMatrix() * m_controller.getTransform();
	  glm::mat4 projectionMatrix = glm::perspective(45.0f, 1.0f, .1f, 10.0f);
	  glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));

	  //  Draw helper geometry
	  m_axis.draw(modelViewMatrix);

	  //  Figure out what shader to use
	  ShaderProgram* m_finalShader;
	  if(Geometry == m_displayMode)
		m_finalShader = &m_finalRenderColor;
	  else
		m_finalShader = &m_finalRenderTexture;

	  //  Draw our geometry
	  m_finalShader->bind();
	  m_finalShader->uniform("modelViewMatrix", modelViewMatrix);
	  m_finalShader->uniform("projectionMatrix", projectionMatrix);
	  m_finalShader->uniform("normalMatrix", normalMatrix);

	  m_captureDecoder->getDepthMap().bind(GL_TEXTURE0);
	  m_captureDecoder->getNormalMap().bind(GL_TEXTURE1);
	  m_captureDecoder->getTextureMap().bind(GL_TEXTURE2);

	  // Draw a plane of pixels
	  m_mesh->draw();
	}
	else if(Depth == m_displayMode)
	{
	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  m_textureDisplay.draw(&m_captureDecoder->getDepthMap());
	}
  }

  m_fpsCalculator.frameUpdate();
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

double CaptureGLWidget::getFrameRate(void)
{
  return m_fpsCalculator.getFrameRate();
}

double CaptureGLWidget::get3DRate(void)
{
  return m_3dpsCalculator.getFrameRate();
}

void CaptureGLWidget::newFringe(IplImage* fringeImage)
{
  if(m_captureDecoder->newImage(fringeImage))
  {
	m_3dpsCalculator.frameUpdate();
	//	Since we could be on another thread, we need to dispatch through Signals/Slots
	emit(crossThreadGLUpdate());
  }
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
