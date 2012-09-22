#include "SixFringeCapture.h"

SixFringeCapture::SixFringeCapture(void)
{
  m_hasBeenInit = false;
  m_haveReferencePhase = false;
  m_captureReferencePhase = false;
  m_currentFringeLoad = 0;
  m_currentChannelLoad = 0;
  m_frontBufferIndex = 0;
  m_gammaCutoff = 0.1f;
  m_scalingFactor = 0.04f;
  m_displayMode = Geometry;
}

SixFringeCapture::~SixFringeCapture()
{
}

void SixFringeCapture::init()
{
  init(256,256);
}

void SixFringeCapture::init(float width, float height)
{
  if(!m_hasBeenInit && width > 0 && height > 0)
  {
    _initShaders(width, height);
    _initTextures(width, height);
    m_textureDisplay.init();
    _initLighting();

    m_axis.init();

    //m_controller.init(512, 512);
	m_controller.init(0.0f, 0.0f, 0.0f, .5f);
    m_camera.init(0.0f, 0.75f, 1.0f, 0.0f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f);
    m_camera.setMode(1);

    m_mesh = shared_ptr<TriMesh>(new TriMesh(width, height));
    m_mesh->initMesh();

	m_fringeLoadingImage = shared_ptr<IplImage>(cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3), [](IplImage* ptr) { cvReleaseImage(&ptr); });

    m_hasBeenInit = true;
  }
}

void SixFringeCapture::resizeInput(float width, float height)
{
  //  Make sure that it has been initalized first.
  if(m_hasBeenInit)
  {
    //  Resize all of the textures
    m_fringeImage1.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage2.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage3.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage4.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

    m_phaseMap0.reinit        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
    m_phaseMap1.reinit        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
    m_depthMap.reinit         (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
    m_normalMap.reinit        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
    m_referencePhase.reinit   (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);

    //  Resize the image processor
    m_imageProcessor.reinit(width, height);
    m_imageProcessor.setTextureAttachPoint(m_phaseMap0, m_phaseMap0AttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_phaseMap1, m_phaseMap1AttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_depthMap, m_depthMapAttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_normalMap, m_normalMapAttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_referencePhase, m_referencePhaseAttachPoint);
    m_imageProcessor.unbind();

    //  Send the new size to all of the shaders
	m_gaussianFilterVertical.uniform("width", width);
	m_gaussianFilterVertical.uniform("height", height);
	m_gaussianFilterHorizontal.uniform("width", width);
	m_gaussianFilterHorizontal.uniform("height", height);

    m_phaseFilter.uniform("width", width);
    m_phaseFilter.uniform("height", height);
    m_normalCalculator.uniform("width", width);
    m_normalCalculator.uniform("height", height);

    //  Resize the display mesh
	m_mesh = shared_ptr<TriMesh>(new TriMesh(width, height));
    m_mesh->initMesh();

    //  Resize the fringe loader
    m_fringeLoadingImage = shared_ptr<IplImage>(cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3), [](IplImage* ptr) { cvReleaseImage(&ptr); });
  }

  OGLStatus::logOGLErrors("SixFringeCapture - resizeInput()");
}

void SixFringeCapture::_initShaders(float width, float height)
{
  //  Kernel used by gaussian filter
  float kernel[11] = {.0495, .0692, .0897, .1081, .1208, .01254, .1208, .1081, .0897, .0692, .0495};

  // Create the shaders
  m_gaussianFilterVertical.init();
  m_gaussianFilterVertical.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/Gaussian11x11Vertical.vert"));
  m_gaussianFilterVertical.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/Gaussian11x11.frag"));
  m_gaussianFilterVertical.bindAttributeLocation("vert", 0);
  m_gaussianFilterVertical.bindAttributeLocation("vertTexCoord", 1);
  m_gaussianFilterVertical.link();
  m_gaussianFilterVertical.uniform("image", 0);
  m_gaussianFilterVertical.uniform("width", width);
  m_gaussianFilterVertical.uniform("height", height);
  m_gaussianFilterVertical.uniform("kernel", kernel, 11); 

  m_gaussianFilterHorizontal.init();
  m_gaussianFilterHorizontal.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/Gaussian11x11Horizontal.vert"));
  m_gaussianFilterHorizontal.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/Gaussian11x11.frag"));
  m_gaussianFilterHorizontal.bindAttributeLocation("vert", 0);
  m_gaussianFilterHorizontal.bindAttributeLocation("vertTexCoord", 1);
  m_gaussianFilterHorizontal.link();
  m_gaussianFilterHorizontal.uniform("image", 0);
  m_gaussianFilterHorizontal.uniform("width", width);
  m_gaussianFilterHorizontal.uniform("height", height);
  m_gaussianFilterHorizontal.uniform("kernel", kernel, 11); 

  m_phaseCalculator.init();
  m_phaseCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/PhaseCalculator.vert"));
  m_phaseCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/PhaseCalculator.frag"));
  m_phaseCalculator.bindAttributeLocation("vert", 0);
  m_phaseCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_phaseCalculator.link();
  m_phaseCalculator.uniform("fringeImage1", 0);
  m_phaseCalculator.uniform("fringeImage2", 1); 
  m_phaseCalculator.uniform("gammaCutoff", m_gammaCutoff);

  m_depthCalculator.init();
  m_depthCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/DepthCalculator.vert"));
  m_depthCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/DepthCalculator.frag"));
  m_depthCalculator.bindAttributeLocation("vert", 0);
  m_depthCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_depthCalculator.link();
  m_depthCalculator.uniform("actualPhase", 0);
  m_depthCalculator.uniform("referencePhase", 1);
  m_depthCalculator.uniform("scalingFactor", m_scalingFactor);

  m_phaseFilter.init();
  m_phaseFilter.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/PhaseFilter.vert"));
  m_phaseFilter.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/PhaseFilter.frag"));
  m_phaseFilter.bindAttributeLocation("vert", 0);
  m_phaseFilter.bindAttributeLocation("vertTexCoord", 1);

  m_phaseFilter.link();
  m_phaseFilter.uniform("image", 0);
  m_phaseFilter.uniform("width", width);
  m_phaseFilter.uniform("height", height);

  m_normalCalculator.init();
  m_normalCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/NormalCalculator.vert"));
  m_normalCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/NormalCalculator.frag"));
  m_normalCalculator.bindAttributeLocation("vert", 0);
  m_normalCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_normalCalculator.link();
  m_normalCalculator.uniform("depthMap", 0);
  m_normalCalculator.uniform("width", width);
  m_normalCalculator.uniform("height", height);

  m_finalRender.init();
  m_finalRender.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/FinalRender.vert"));
  m_finalRender.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/FinalRender.frag"));
  m_finalRender.bindAttributeLocation("vert", 0);
  m_finalRender.bindAttributeLocation("vertTexCoord", 1);

  m_finalRender.link();
  m_finalRender.uniform("normals", 0);
  m_finalRender.uniform("depthMap", 1);
  m_finalRender.uniform("phaseMap", 2);

  OGLStatus::logOGLErrors("SixFringeCapture - initShaders()");
}

void SixFringeCapture::_initTextures(GLuint width, GLuint height)
{
  Logger::logDebug("SixFringeCapture - initTextures(): Creating textures for phase map and normal map");

  m_phaseMap0AttachPoint      = GL_COLOR_ATTACHMENT0_EXT;
  m_phaseMap1AttachPoint      = GL_COLOR_ATTACHMENT1_EXT;
  m_depthMapAttachPoint       = GL_COLOR_ATTACHMENT2_EXT;
  m_normalMapAttachPoint      = GL_COLOR_ATTACHMENT3_EXT;
  m_referencePhaseAttachPoint = GL_COLOR_ATTACHMENT4_EXT;

  m_fringeImage1.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage2.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage3.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage4.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  
  m_fringeImages[0][0] = &m_fringeImage1;
  m_fringeImages[0][1] = &m_fringeImage2; 
  m_fringeImages[1][0] = &m_fringeImage3;
  m_fringeImages[1][1] = &m_fringeImage4;

  m_phaseMap0.init        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_phaseMap1.init        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_depthMap.init         (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_normalMap.init        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_referencePhase.init   (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);

  m_imageProcessor.init(width, height);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap0, m_phaseMap0AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap1, m_phaseMap1AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_depthMap, m_depthMapAttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_normalMap, m_normalMapAttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_referencePhase, m_referencePhaseAttachPoint);
  m_imageProcessor.unbind();

  OGLStatus::logOGLErrors("SixFringeCapture - initTextures()");
}

void SixFringeCapture::_initLighting(void)
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  m_finalRender.uniform("lightPosition", glm::vec3(0.5f, 0.5f, 4.0f));
  m_finalRender.uniform("ambientColor", glm::vec4(.1, .1, .1, 1.0));
  m_finalRender.uniform("diffuseColor", glm::vec4(1.0, 1.0, 1.0, 1.0));
  m_finalRender.uniform("specularColor", glm::vec4(1.0, 1.0, 1.0, 1.0));
}

void SixFringeCapture::setGammaCutoff(float gamma)
{
  m_gammaCutoff = gamma;
}

void SixFringeCapture::setScalingFactor(float scalingFactor)
{
  m_scalingFactor = scalingFactor;
}

MeshInterchange* SixFringeCapture::decode(void)
{
  OGLStatus::logOGLErrors("SixFringeCapture - decode()");
  return new MeshInterchange(m_depthMap);
}

void SixFringeCapture::draw(void)
{
  if(m_captureReferencePhase)
  {
    //  If we dont have the reference phase then we are calculating it and we redraw
    m_imageProcessor.bind();
    {
      m_imageProcessor.bindDrawBuffer(m_phaseMap0AttachPoint);
      m_phaseCalculator.bind();
      m_fringeImages[m_frontBufferIndex][0]->bind(GL_TEXTURE0);
      m_fringeImages[m_frontBufferIndex][1]->bind(GL_TEXTURE1); 
      m_imageProcessor.process();

	  m_imageProcessor.bindDrawBuffer(m_phaseMap1AttachPoint);
	  m_gaussianFilterVertical.bind();
	  m_phaseMap0.bind(GL_TEXTURE0);
	  m_imageProcessor.process();

	  m_imageProcessor.bindDrawBuffer(m_referencePhaseAttachPoint);
	  m_gaussianFilterHorizontal.bind();
	  m_phaseMap1.bind(GL_TEXTURE0);
	  m_imageProcessor.process();
    }
    m_imageProcessor.unbind();

    m_haveReferencePhase = true;
    m_captureReferencePhase = false;
  }
  else if(m_haveReferencePhase && Geometry == m_displayMode)
  {
	m_imageProcessor.bind();
	{
      //	Pass 1 - Phase Calculation
      _drawCalculatePhase();

      //	Pass 2 - Phase filtering
      _drawFilterPhase();

      //    Pass 3 - Depth Calculator
      _drawCalculateDepthMap();

      //	Pass 4 - Normal Calculation
      _drawCalculateNormalMap();
	}
    m_imageProcessor.unbind();

	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();

    m_camera.applyMatrix();
    m_controller.applyTransform();

	glColor3f(0.0f, 1.0f, 0.0f);

    glm::mat4 modelViewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 normalMatrix;

    glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelViewMatrix));
    glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projectionMatrix));
    normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));   //  This is needed for lighting calculations

    m_axis.draw(modelViewMatrix);

	m_finalRender.bind();
	{
      m_finalRender.uniform("modelViewMatrix", modelViewMatrix);
      m_finalRender.uniform("projectionMatrix", projectionMatrix);
      m_finalRender.uniform("normalMatrix", normalMatrix);

      m_normalMap.bind(GL_TEXTURE0);
      m_depthMap.bind(GL_TEXTURE1);
      m_phaseMap0.bind(GL_TEXTURE2);

      // Draw a plane of pixels
      m_mesh->draw();
	}
	m_finalRender.unbind();

    glPopMatrix();
  }
  else if(m_haveReferencePhase && Phase == m_displayMode)
  {
    m_textureDisplay.draw(&m_referencePhase);
  }

  m_fpsCalculator.frameUpdate();
  OGLStatus::logOGLErrors("SixFringeCapture - draw()");
}

void SixFringeCapture::resize(int width, int height)
{
  m_camera.reshape(width, height);
  m_textureDisplay.resize(width, height);

  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, 1.0, .00001, 1000.0);
  glMatrixMode(GL_MODELVIEW);
}

void SixFringeCapture::cameraSelectMode(int mode)
{
  m_camera.setMode(mode);
}

void SixFringeCapture::mousePressEvent(int mouseX, int mouseY)
{
  m_camera.mousePressed(mouseX, mouseY);
}

void SixFringeCapture::mouseMoveEvent(int mouseX, int mouseY)
{
  m_camera.mouseMotion(mouseX, mouseY);
}

bool SixFringeCapture::newImage(IplImage* image)
{
  bool needRedraw = false;

  cvSetImageCOI(image, 1);
  cvSetImageCOI(m_fringeLoadingImage.get(), (m_currentChannelLoad + 1));
  cvCopy(image, m_fringeLoadingImage.get());
  cvSetImageCOI(m_fringeLoadingImage.get(), 0);
  cvSetImageCOI(image, 0);

  m_currentChannelLoad++;

  if(m_currentChannelLoad == 3)
  {
    int backBufferIndex = (m_frontBufferIndex + 1) % 2;
    m_fringeImages[backBufferIndex][m_currentFringeLoad]->transferToTexture(m_fringeLoadingImage.get());
    m_currentChannelLoad = 0;
    m_currentFringeLoad++;
  }

  if(m_currentFringeLoad == 2)
  {
	  //  Capture first 6 images as reference plane
	if(!m_haveReferencePhase)
	{
	  m_captureReferencePhase = true;
	}

    m_currentChannelLoad = 0;
    m_currentFringeLoad = 0;
    swapBuffers();
    m_3dpsCalculator.frameUpdate();
    needRedraw = true;
  }

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("SixFringeCapture - setBackHoloBuffer()");
  return needRedraw;
}

void SixFringeCapture::swapBuffers(void)
{
  //	Switch the front and back buffer
  m_frontBufferIndex = (m_frontBufferIndex + 1) % 2;

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("SixFringeCapture - swapBuffers()");
}

void SixFringeCapture::captureReferencePlane(void)
{
  m_captureReferencePhase = true;
}

void SixFringeCapture::show3D(void)
{
  m_displayMode = Geometry;
}

void SixFringeCapture::showPhase(void)
{
  m_displayMode = Phase;
}

double SixFringeCapture::getFrameRate(void)
{
  return m_fpsCalculator.getFrameRate();
}

double SixFringeCapture::get3DRate(void)
{
  return m_3dpsCalculator.getFrameRate();
}

string SixFringeCapture::getCaptureName(void)
{
  return "Six Fringe Capture";
}

void SixFringeCapture::_drawCalculatePhase()
{
  m_imageProcessor.bindDrawBuffer(m_phaseMap0AttachPoint);
  m_phaseCalculator.bind();
  m_phaseCalculator.uniform("gammaCutoff", m_gammaCutoff);
  m_fringeImages[m_frontBufferIndex][0]->bind(GL_TEXTURE0);
  m_fringeImages[m_frontBufferIndex][1]->bind(GL_TEXTURE1); 
  m_imageProcessor.process();
}

void SixFringeCapture::_drawFilterPhase()
{
  m_imageProcessor.bindDrawBuffer(m_phaseMap1AttachPoint);
  m_phaseFilter.bind();
  m_phaseMap0.bind(GL_TEXTURE0);
  m_imageProcessor.process();

  //	Pass 2 - Phase filtering ... again
  m_imageProcessor.bindDrawBuffer(m_phaseMap0AttachPoint);
  m_phaseFilter.bind();
  m_phaseMap1.bind(GL_TEXTURE0);
  m_imageProcessor.process();
}

void SixFringeCapture::_drawCalculateDepthMap()
{
  m_imageProcessor.bindDrawBuffer(m_depthMapAttachPoint);
  m_depthCalculator.uniform("scalingFactor", m_scalingFactor);
  m_depthCalculator.bind();
  m_phaseMap0.bind(GL_TEXTURE0);
  m_referencePhase.bind(GL_TEXTURE1);
  m_imageProcessor.process();
}

void SixFringeCapture::_drawCalculateNormalMap()
{
  m_imageProcessor.bindDrawBuffer(m_normalMapAttachPoint);
  m_normalCalculator.bind();
  m_depthMap.bind(GL_TEXTURE0);
  m_imageProcessor.process();
}
