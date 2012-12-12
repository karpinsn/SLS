#include "NineFringeCapture.h"

NineFringeCapture::NineFringeCapture(void)
{
  m_hasBeenInit = false;
  m_haveReferencePhase = false;
  m_captureReferencePhase = false;
  m_currentFringeLoad = 0;
  m_currentChannelLoad = 0;
  m_frontBufferIndex = 0;
  m_gammaCutoff = 0.1f;
  m_scalingFactor = 0.04f;
  m_shiftFactor = 0.0f;
}

void NineFringeCapture::init()
{
  init(256,256);
}

void NineFringeCapture::init(int width, int height)
{
  if(!m_hasBeenInit && width > 0 && height > 0)
  {
	  m_width = width;
	  m_height = height;

    _initShaders(width, height);
    _initTextures(width, height);

	m_fringeLoadingImage = shared_ptr<IplImage>(cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3), [](IplImage* ptr) { cvReleaseImage(&ptr); });

    m_hasBeenInit = true;
  }
}

int NineFringeCapture::getWidth()
{
	return m_width;
}

int NineFringeCapture::getHeight()
{
	return m_height;
}

void NineFringeCapture::resizeInput(int width, int height)
{
  //  Make sure that it has been initalized first.
  if(m_hasBeenInit && width > 0 && height > 0 && (width != m_width || height != m_height))
  {
	  m_width = width;
	  m_height = height;

    //  Resize all of the textures
    m_fringeImage1.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage2.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage3.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage4.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage5.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage6.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

    m_phaseMap0.reinit        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
    m_phaseMap1.reinit        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
    m_depthMap.reinit         (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
    m_normalMap.reinit        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
	m_textureMap.reinit		  (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_referencePhase.reinit   (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);

    //  Resize the image processor
    m_imageProcessor.reinit(width, height);
    m_imageProcessor.setTextureAttachPoint(m_phaseMap0, m_phaseMap0AttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_phaseMap1, m_phaseMap1AttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_depthMap, m_depthMapAttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_normalMap, m_normalMapAttachPoint);
	m_imageProcessor.setTextureAttachPoint(m_textureMap, m_textureMapAttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_referencePhase, m_referencePhaseAttachPoint);
    m_imageProcessor.unbind();

	//  Send the new size to all of the shaders
	m_gaussianFilterVertical.uniform("width", (float)width);
	m_gaussianFilterVertical.uniform("height", (float)height);
	m_gaussianFilterHorizontal.uniform("width", (float)width);
	m_gaussianFilterHorizontal.uniform("height", (float)height);

    //  Send the new size to all of the shaders
    m_phaseFilter.uniform("width", (float)width);
    m_phaseFilter.uniform("height", (float)height);
    m_normalCalculator.uniform("width", (float)width);
    m_normalCalculator.uniform("height", (float)height);

    //  Resize the fringe loader
	m_fringeLoadingImage = shared_ptr<IplImage>(cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3), [](IplImage* ptr) { cvReleaseImage(&ptr); });
  }

  OGLStatus::logOGLErrors("NineFringeCapture - resizeInput()");
}

void NineFringeCapture::_initShaders(float width, float height)
{
  //  Kernel used by gaussian filter
  float kernel[11] = {.0495, .0692, .0897, .1081, .1208, .1254, .1208, .1081, .0897, .0692, .0495};

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
  m_phaseCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/NineFringe/PhaseCalculator.vert"));
  m_phaseCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/NineFringe/PhaseCalculator.frag"));
  m_phaseCalculator.bindAttributeLocation("vert", 0);
  m_phaseCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_phaseCalculator.link();
  m_phaseCalculator.uniform("fringeImage1", 0);
  m_phaseCalculator.uniform("fringeImage2", 1);
  m_phaseCalculator.uniform("fringeImage3", 2);
  m_phaseCalculator.uniform("gammaCutoff", m_gammaCutoff);

  m_depthCalculator.init();
  m_depthCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/NineFringe/DepthCalculator.vert"));
  m_depthCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/NineFringe/DepthCalculator.frag"));
  m_depthCalculator.bindAttributeLocation("vert", 0);
  m_depthCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_depthCalculator.link();
  m_depthCalculator.uniform("actualPhase", 0);
  m_depthCalculator.uniform("referencePhase", 1);
  m_depthCalculator.uniform("textureMap", 2);
  m_depthCalculator.uniform("scalingFactor", m_scalingFactor);
  m_depthCalculator.uniform("shiftFactor", m_shiftFactor);

  m_phaseFilter.init();
  m_phaseFilter.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/NineFringe/VerticalPhaseFilter.vert"));
  m_phaseFilter.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/NineFringe/VerticalPhaseFilter.frag"));
  m_phaseFilter.bindAttributeLocation("vert", 0);
  m_phaseFilter.bindAttributeLocation("vertTexCoord", 1);

  m_phaseFilter.link();
  m_phaseFilter.uniform("image", 0);
  m_phaseFilter.uniform("width", width);
  m_phaseFilter.uniform("height", height);

  m_normalCalculator.init();
  m_normalCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/NineFringe/NormalCalculator.vert"));
  m_normalCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/NineFringe/NormalCalculator.frag"));
  m_normalCalculator.bindAttributeLocation("vert", 0);
  m_normalCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_normalCalculator.link();
  m_normalCalculator.uniform("depthMap", 0);
  m_normalCalculator.uniform("width", width);
  m_normalCalculator.uniform("height", height);

  m_textureCalculator.init();
  m_textureCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/NineFringe/TextureCalculator.vert"));
  m_textureCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/NineFringe/TextureCalculator.frag"));
  m_textureCalculator.bindAttributeLocation("vert", 0);
  m_textureCalculator.bindAttributeLocation("vertTexCoord", 1);
  m_textureCalculator.link();
  m_phaseCalculator.uniform("fringe", 0);

  OGLStatus::logOGLErrors("NineFringeCapture - initShaders()");
}

void NineFringeCapture::_initTextures(GLuint width, GLuint height)
{
  Logger::logDebug("NineFringeCapture - initTextures(): Creating textures for phase map and normal map");

  m_phaseMap0AttachPoint      = GL_COLOR_ATTACHMENT0;
  m_phaseMap1AttachPoint      = GL_COLOR_ATTACHMENT1;
  m_depthMapAttachPoint       = GL_COLOR_ATTACHMENT2;
  m_normalMapAttachPoint      = GL_COLOR_ATTACHMENT3;
  m_textureMapAttachPoint	  = GL_COLOR_ATTACHMENT4;
  m_referencePhaseAttachPoint = GL_COLOR_ATTACHMENT5;

  m_fringeImage1.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage2.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage3.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage4.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage5.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage6.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

  m_fringeImages[0][0] = &m_fringeImage1;
  m_fringeImages[0][1] = &m_fringeImage2;
  m_fringeImages[0][2] = &m_fringeImage3;
  m_fringeImages[1][0] = &m_fringeImage4;
  m_fringeImages[1][1] = &m_fringeImage5;
  m_fringeImages[1][2] = &m_fringeImage6;

  m_phaseMap0.init        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_phaseMap1.init        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_depthMap.init         (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_normalMap.init        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_textureMap.init		  (width, height, GL_RGB,		 GL_RGB, GL_UNSIGNED_BYTE);
  m_referencePhase.init   (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);

  m_imageProcessor.init(width, height);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap0, m_phaseMap0AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap1, m_phaseMap1AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_depthMap, m_depthMapAttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_normalMap, m_normalMapAttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_textureMap, m_textureMapAttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_referencePhase, m_referencePhaseAttachPoint);
  m_imageProcessor.unbind();

  OGLStatus::logOGLErrors("NineFringeCapture - initTextures()");
}

void NineFringeCapture::setGammaCutoff(float gamma)
{
  m_gammaCutoff = gamma;
}

void NineFringeCapture::setBlackLevel(float blackLevel)
{
  m_blackLevel = blackLevel;
}

void NineFringeCapture::setScalingFactor(float scalingFactor)
{
  m_scalingFactor = scalingFactor;
}

void NineFringeCapture::setShiftFactor(float shiftFactor)
{
  m_shiftFactor = shiftFactor;
}

void NineFringeCapture::decode(void)
{
  OGLStatus::logOGLErrors("NineFringeCapture - decode()");
  draw();
}

Texture& NineFringeCapture::getDepthMap(void)
{
  return m_depthMap;
}

Texture& NineFringeCapture::getTextureMap(void)
{
  return m_textureMap;
}

Texture& NineFringeCapture::getNormalMap(void)
{
  return m_normalMap;
}

void NineFringeCapture::draw(void)
{
  if(m_captureReferencePhase)
  {
    //  If we dont have the reference phase then we are calculating it and we redraw
    m_imageProcessor.bind();
    {
	  // Pass 1 - Phase Calculation
	  // Save the gamma so we can restore it
	  float currentGamma = m_gammaCutoff;
	  m_gammaCutoff = 0.0f;
      _drawCalculatePhase();
	  m_gammaCutoff = currentGamma;

	  m_imageProcessor.bindDrawBuffer(m_phaseMap1AttachPoint);
	  m_gaussianFilterVertical.bind();
	  m_phaseMap0.bind(GL_TEXTURE0);
	  m_imageProcessor.process();

	  m_imageProcessor.bindDrawBuffer(m_phaseMap0AttachPoint);
	  m_gaussianFilterHorizontal.bind();
	  m_phaseMap1.bind(GL_TEXTURE0);
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
  else if(m_haveReferencePhase)
  {
	m_imageProcessor.bind();
	{
      //	Pass 1 - Phase Calculation
      _drawCalculatePhase();

      //	Pass 2 - Phase filtering
      _drawFilterPhase();

	  //	Pass 3 - Texture calculation
	  _drawCalculateTexture();

      //    Pass 3 - Depth Calculator
      _drawCalculateDepthMap();

      //	Pass 4 - Normal Calculation
      _drawCalculateNormalMap();
	}
    m_imageProcessor.unbind();
  }

  OGLStatus::logOGLErrors("NineFringeCapture - draw()");
}

int NineFringeCapture::getFringeCount()
{
  return 9;
}

bool NineFringeCapture::newImage(IplImage* image)
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

  if(m_currentFringeLoad == 3)
  {
    m_currentChannelLoad = 0;
    m_currentFringeLoad = 0;
    swapBuffers();
    needRedraw = true;
  }

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("NineFringeCapture - setBackHoloBuffer()");
  return needRedraw;
}

void NineFringeCapture::swapBuffers(void)
{
  //	Switch the front and back buffer
  m_frontBufferIndex = (m_frontBufferIndex + 1) % 2;

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("NineFringeCapture - swapBuffers()");
}

void NineFringeCapture::loadReferencePlane(void* callbackInstance, shared_ptr<IplImage> (*imageLoaderFunction)(void* callbackInstance))
{
  shared_ptr<IplImage> fringe1 = imageLoaderFunction(callbackInstance);
  shared_ptr<IplImage> fringe2 = imageLoaderFunction(callbackInstance);
  shared_ptr<IplImage> fringe3 = imageLoaderFunction(callbackInstance);

  //  If we didn't get our fringes just abort
  if(nullptr == fringe1 || nullptr == fringe2 || nullptr == fringe3)
  {
	return;
  }

  //  We need to resize according to our image size
  resizeInput(fringe1->width, fringe1->height);

  //  Signify that we want to capture the reference plane
  captureReferencePlane();

  int backBufferIndex = (m_frontBufferIndex + 1) % 2;
  m_fringeImages[backBufferIndex][0]->transferToTexture(fringe1.get());
  m_fringeImages[backBufferIndex][1]->transferToTexture(fringe2.get());
  m_fringeImages[backBufferIndex][2]->transferToTexture(fringe3.get());

  swapBuffers();
}

void NineFringeCapture::captureReferencePlane(void)
{
  m_captureReferencePhase = true;
}

string NineFringeCapture::getCaptureName(void)
{
  return "Nine Fringe Capture";
}

void NineFringeCapture::_drawCalculatePhase()
{
  m_imageProcessor.bindDrawBuffer(m_phaseMap0AttachPoint);
  m_phaseCalculator.bind();
  m_phaseCalculator.uniform("gammaCutoff", m_gammaCutoff);
  m_fringeImages[m_frontBufferIndex][0]->bind(GL_TEXTURE0);
  m_fringeImages[m_frontBufferIndex][1]->bind(GL_TEXTURE1);
  m_fringeImages[m_frontBufferIndex][2]->bind(GL_TEXTURE2);
  m_imageProcessor.process();
}

void NineFringeCapture::_drawFilterPhase()
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

void NineFringeCapture::_drawCalculateTexture()
{
  m_imageProcessor.bindDrawBuffer(m_textureMapAttachPoint);
  m_textureCalculator.bind();
  m_fringeImages[m_frontBufferIndex][0]->bind(GL_TEXTURE0);
  m_imageProcessor.process();
}

void NineFringeCapture::_drawCalculateDepthMap()
{
  m_imageProcessor.bindDrawBuffer(m_depthMapAttachPoint);
  m_depthCalculator.uniform("scalingFactor", m_scalingFactor);
  m_depthCalculator.uniform("shiftFactor", m_shiftFactor);
  m_depthCalculator.uniform("blackLevelThreshold", m_blackLevel);
  m_depthCalculator.bind();
  m_phaseMap0.bind(GL_TEXTURE0);
  m_referencePhase.bind(GL_TEXTURE1);
  m_textureMap.bind(GL_TEXTURE2);
  m_imageProcessor.process();
}

void NineFringeCapture::_drawCalculateNormalMap()
{
  m_imageProcessor.bindDrawBuffer(m_normalMapAttachPoint);
  m_normalCalculator.bind();
  m_depthMap.bind(GL_TEXTURE0);
  m_imageProcessor.process();
}
