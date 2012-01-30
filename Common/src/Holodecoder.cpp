#include "Holodecoder.h"

Holodecoder::Holodecoder(void)
{
  haveHoloImage = false;
  m_hasBeenInit = false;
}

void Holodecoder::init()
{
  init(512,512);
}

void Holodecoder::init(float width, float height)
{
  if(!m_hasBeenInit)
  {
	initShaders();

	_initTextures(width, height);
	_initLighting();

	m_controller.init(512, 512);
	m_camera.init(0.0f, 0.75f, 1.0f, 0.0f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f);
	m_camera.setMode(1);

	m_mesh = new TriMesh(512, 512);

	m_mesh->initMesh();
	haveHoloImage = false;
	m_hasBeenInit = true;

	m_holoImages[0] = &m_holoImage0;
	m_holoImages[1] = &m_holoImage1;

	GLfloat topColor[3] = {.8, .8, .8};
	GLfloat bottomColor[3] = {.7, .7, .7};
	m_background.setColors(topColor, bottomColor);
  }
}

void Holodecoder::initShaders(void)
{
  //	Create the shaders
  m_phaseCalculator.init();
  m_phaseCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/Holovideo/PhaseCalculator.vert"));
  m_phaseCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/Holovideo/PhaseCalculator.frag"));
  m_phaseCalculator.bindAttributeLocation("vert", 0);
  m_phaseCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_phaseCalculator.link();
  m_phaseCalculator.uniform("holoImage", 0);

  m_phaseFilter.init();
  m_phaseFilter.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/Holovideo/PhaseFilter.vert"));
  m_phaseFilter.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/Holovideo/PhaseFilter.frag"));
  m_phaseFilter.bindAttributeLocation("vert", 0);
  m_phaseFilter.bindAttributeLocation("vertTexCoord", 1);

  m_phaseFilter.link();
  m_phaseFilter.uniform("image", 0);
  m_phaseFilter.uniform("width", 512.0f);
  m_phaseFilter.uniform("height", 512.0f);

  m_depthCalculator.init();
  m_depthCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/Holovideo/DepthCalculator.vert"));
  m_depthCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/Holovideo/DepthCalculator.frag"));
  m_depthCalculator.bindAttributeLocation("vert", 0);
  m_depthCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_depthCalculator.link();
  m_depthCalculator.uniform("phaseMap", 0);
  m_depthCalculator.uniform("width", 512.0f);

  m_normalCalculator.init();
  m_normalCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/NormalCalculator.vert"));
  m_normalCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/NormalCalculator.frag"));
  m_normalCalculator.bindAttributeLocation("vert", 0);
  m_normalCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_normalCalculator.link();
  m_normalCalculator.uniform("depthMap", 0);
  m_normalCalculator.uniform("width", 512.0f);
  m_normalCalculator.uniform("height", 512.0f);

  m_finalRender.init();
  m_finalRender.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/Holovideo/FinalRender.vert"));
  m_finalRender.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/Holovideo/FinalRender.frag"));
  m_finalRender.bindAttributeLocation("vert", 0);
  m_finalRender.bindAttributeLocation("vertTexCoord", 1);

  m_finalRender.link();
  m_finalRender.uniform("normals", 0);
  m_finalRender.uniform("depthMap", 1);
  m_finalRender.uniform("holoImage", 2);
  m_finalRender.uniform("width", 512.0f);
  m_finalRender.unbind();

  OGLStatus::logOGLErrors("Holodecoder - initShaders()");
}

void Holodecoder::_initTextures(GLuint width, GLuint height)
{
  Logger::logDebug("Holodecoder - initTextures(): Creating textures for phase map and normal map");

  m_phaseMap0AttachPoint  = GL_COLOR_ATTACHMENT0_EXT;
  m_phaseMap1AttachPoint  = GL_COLOR_ATTACHMENT1_EXT;
  m_depthMapAttachPoint   = GL_COLOR_ATTACHMENT2_EXT;
  m_normalMapAttachPoint  = GL_COLOR_ATTACHMENT3_EXT;

  m_holoImage0.init   (width, height, GL_RGB, GL_BGR, GL_UNSIGNED_BYTE);
  m_holoImage1.init   (width, height, GL_RGB, GL_BGR, GL_UNSIGNED_BYTE);
  m_phaseMap0.init    (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_phaseMap1.init    (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_depthMap.init     (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_normalMap.init    (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);

  m_imageProcessor.init(width, height);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap0, m_phaseMap0AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap1, m_phaseMap1AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_depthMap, m_depthMapAttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_normalMap, m_normalMapAttachPoint);
  m_imageProcessor.unbind();

  OGLStatus::logOGLErrors("Holodecoder - initTextures()");
}

void Holodecoder::draw(void)
{
  //m_background.draw();

  if(haveHoloImage)
  {
	m_imageProcessor.bind();
	{
	  //	Pass 1
	  _drawCalculatePhase();

	  //	Pass 2
	  _drawFilterPhase();

	  //  Pass 3
	  _drawCalculateDepthMap();

	  //  Pass 4
	  m_imageProcessor.bindDrawBuffer(m_normalMapAttachPoint);
	  m_normalCalculator.bind();
	  m_depthMap.bind(GL_TEXTURE0);
	  m_imageProcessor.process();
	  m_normalCalculator.unbind();
	}
	m_imageProcessor.unbind();

	glMatrixMode(GL_MODELVIEW);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glLoadIdentity();
	m_camera.applyMatrix();
	m_background.draw();

	glColor3f(0.8f, 0.8f, 0.8f);
	
	m_finalRender.bind();
	{
	  m_normalMap.bind(GL_TEXTURE0);
	  m_depthMap.bind(GL_TEXTURE1);
	  m_holoImages[m_frontBufferIndex]->bind(GL_TEXTURE2);

	  // Draw a plane of pixels
	  m_controller.applyTransform();
	  m_mesh->draw();
	}
	m_finalRender.unbind();
	
	glPopMatrix();
  }
  OGLStatus::logOGLErrors("Holodecoder - draw()");
}


MeshInterchange* Holodecoder::decode(void)
{
  OGLStatus::logOGLErrors("Holodecoder - decode()");
  return new MeshInterchange(m_depthMap);
}

void Holodecoder::resize(int width, int height)
{
  gluPerspective(45.0, (float)width/(float)height, .00001, 10.0);

  m_camera.reshape(width, height);

  m_background.resize(width, height);
}

void Holodecoder::cameraSelectMode(int mode)
{
  m_camera.setMode(mode);
}

void Holodecoder::mousePressEvent(int mouseX, int mouseY)
{
  m_camera.mousePressed(mouseX, mouseY);
}

void Holodecoder::mouseMoveEvent(int mouseX, int mouseY)
{
  m_camera.mouseMotion(mouseX, mouseY);
}

void Holodecoder::setBackHoloBuffer(IplImage* image)
{
  int backBufferIndex = (m_frontBufferIndex + 1) % 2;
  m_holoImages[backBufferIndex]->transferToTexture(image);

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("Holodecoder - setBackHoloBuffer()");
}

void Holodecoder::setBackHoloBuffer(reactor::MediaFrame& frame)
{
  int backBufferIndex = (m_frontBufferIndex + 1) % 2;
  m_holoImages[backBufferIndex]->transferToTexture(frame);

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("Holodecoder - setBackHoloBuffer()");
}

void Holodecoder::swapBuffers(void)
{
  //	Switch the front and back buffer
  m_frontBufferIndex = (m_frontBufferIndex + 1) % 2;

  haveHoloImage = true;

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("Holodecoder - swapBuffers()");
}

void Holodecoder::_drawCalculatePhase()
{
  m_imageProcessor.bindDrawBuffer(m_phaseMap0AttachPoint);
  m_phaseCalculator.bind();
  m_holoImages[m_frontBufferIndex]->bind(GL_TEXTURE0);
  m_imageProcessor.process();
}

void Holodecoder::_drawFilterPhase()
{
  m_imageProcessor.bindDrawBuffer(m_phaseMap1AttachPoint);
  m_phaseFilter.bind();
  m_phaseMap0.bind(GL_TEXTURE0);
  m_imageProcessor.process();
}

void Holodecoder::_drawCalculateDepthMap()
{
  m_imageProcessor.bindDrawBuffer(m_depthMapAttachPoint);
  m_depthCalculator.bind();
  m_phaseMap1.bind(GL_TEXTURE0);
  m_imageProcessor.process();
}

void Holodecoder::_initLighting(void)
{
  GLfloat mat_specular[] = {.1f, .1f, .1f, .1f};
  GLfloat mat_shininess[] = {1.0f};
  GLfloat light_position[] = {-2.0f, 6.0f, 8.0f, 0.0f};
  GLfloat white_light[] = {1.0f, 1.0f, 1.0f, 1.0f};

  //glClearColor(.1914, .3007, .4727, 0.0);

  glShadeModel(GL_SMOOTH);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  //	Setup light 0
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

  //	Enable lighting
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}
