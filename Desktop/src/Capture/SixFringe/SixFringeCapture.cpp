#include "SixFringeCapture.h"

SixFringeCapture::SixFringeCapture(void) : m_gaussFilter( 7 ), m_hasBeenInit( false ), 
  m_haveReferencePhase( false ), m_captureReferencePhase( false ), m_currentFringeLoad( 0 ), m_currentChannelLoad(0),
  m_frontBufferIndex( 0 ), m_gammaCutoff( .1f ), m_scalingFactor( .04f ), m_shiftFactor( 0.0f )
{ }

void SixFringeCapture::init()
  { init( 256, 256 ); }

void SixFringeCapture::init(int width, int height)
{
  if(!m_hasBeenInit && width > 0 && height > 0)
  {
    _initShaders(width, height);
    _initTextures(width, height);

	m_fringeLoadingImage = shared_ptr<IplImage>(cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3), [](IplImage* ptr) { cvReleaseImage(&ptr); });

	m_width = width;
	m_height = height;

    m_hasBeenInit = true;
  }
}

int SixFringeCapture::getWidth()
  { return m_width; }

int SixFringeCapture::getHeight()
  { return m_height; }

void SixFringeCapture::resizeInput(int width, int height)
{
  //  Make sure that it has been initalized first.
  if(m_hasBeenInit && width > 0 && height > 0 && (m_width != width || m_height != height))
  {
    //  Resize all of the textures
    m_fringeImage1.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage2.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage3.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
    m_fringeImage4.reinit     (width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

    m_phaseMap0.reinit        (width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
    m_phaseMap1.reinit        (width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
	m_phaseMap2.reinit        (width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
    m_depthMap.reinit         (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
    m_normalMap.reinit        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
    m_referencePhase.reinit   (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);

    //  Resize the image processor
    m_imageProcessor.reinit(width, height);
    m_imageProcessor.setTextureAttachPoint(m_phaseMap0, m_phaseMap0AttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_phaseMap1, m_phaseMap1AttachPoint);
	m_imageProcessor.setTextureAttachPoint(m_phaseMap2, m_phaseMap2AttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_depthMap, m_depthMapAttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_normalMap, m_normalMapAttachPoint);
    m_imageProcessor.setTextureAttachPoint(m_referencePhase, m_referencePhaseAttachPoint);
    m_imageProcessor.unbind();

    //  Send the new size to all of the shaders
	m_gaussFilter.setImageDimensions( width, height );

    m_phaseFilter.uniform("width", (float)width);
    m_phaseFilter.uniform("height", (float)height);
    m_normalCalculator.uniform("width", (float)width);
    m_normalCalculator.uniform("height", (float)height);

	m_width = width;
	m_height = height;

    //  Resize the fringe loader
    m_fringeLoadingImage = shared_ptr<IplImage>(cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3), [](IplImage* ptr) { cvReleaseImage(&ptr); });
  }

  OGLStatus::logOGLErrors("SixFringeCapture - resizeInput()");
}

void SixFringeCapture::_initShaders(float width, float height)
{
  // Pass 1 - Phase Wrapping
  m_phaseWrapper.init();
  m_phaseWrapper.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/PassThrough.vert"));
  m_phaseWrapper.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/PhaseWrapper.frag"));
  m_phaseWrapper.bindAttributeLocation("vert", 0);
  m_phaseWrapper.bindAttributeLocation("vertTexCoord", 1);
  m_phaseWrapper.link();
  m_phaseWrapper.uniform("fringeImage1", 0);
  m_phaseWrapper.uniform("fringeImage2", 1); 
  m_phaseWrapper.uniform("gammaCutoff", m_gammaCutoff);

  // Pass 2 - Phase Filtering
  m_gaussFilter.init( );
  m_gaussFilter.setImageDimensions( width, height );

  m_phaseUnwrapper.init( );
  m_phaseUnwrapper.attachShader( new Shader( GL_VERTEX_SHADER, "Shaders/SixFringe/PassThrough.vert" ) );
  m_phaseUnwrapper.attachShader( new Shader( GL_FRAGMENT_SHADER, "Shaders/SixFringe/PhaseUnwrapper.frag" ) );
  m_phaseUnwrapper.bindAttributeLocation( "vert", 0 );
  m_phaseUnwrapper.bindAttributeLocation( "vertTexCoord", 1 );
  m_phaseUnwrapper.link( );
  m_phaseUnwrapper.uniform( "pitch1", 48 );
  m_phaseUnwrapper.uniform( "pitch2", 57 );
  m_phaseUnwrapper.uniform( "unfilteredWrappedPhase", 0 );
  m_phaseUnwrapper.uniform( "filteredWrappedPhase", 1 );

  m_phaseFilter.init();
  m_phaseFilter.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/PassThrough.vert"));
  m_phaseFilter.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/PhaseFilter.frag"));
  m_phaseFilter.bindAttributeLocation("vert", 0);
  m_phaseFilter.bindAttributeLocation("vertTexCoord", 1);

  m_phaseFilter.link();
  m_phaseFilter.uniform("image", 0);
  m_phaseFilter.uniform("width", width);
  m_phaseFilter.uniform("height", height);

  // Pass 3 - Depth Calculation
  m_depthCalculator.init();
  m_depthCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/PassThrough.vert"));
  m_depthCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/DepthCalculator.frag"));
  m_depthCalculator.bindAttributeLocation("vert", 0);
  m_depthCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_depthCalculator.link();
  m_depthCalculator.uniform("actualPhase", 0);
  m_depthCalculator.uniform("referencePhase", 1);
  m_depthCalculator.uniform("scalingFactor", m_scalingFactor);

  // Pass 4 - Normal Calculation
  m_normalCalculator.init();
  m_normalCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SixFringe/PassThrough.vert"));
  m_normalCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SixFringe/NormalCalculator.frag"));
  m_normalCalculator.bindAttributeLocation("vert", 0);
  m_normalCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_normalCalculator.link();
  m_normalCalculator.uniform("depthMap", 0);
  m_normalCalculator.uniform("width", width);
  m_normalCalculator.uniform("height", height);

  OGLStatus::logOGLErrors("SixFringeCapture - initShaders()");
}

void SixFringeCapture::_initTextures(GLuint width, GLuint height)
{
  Logger::logDebug("SixFringeCapture - initTextures(): Creating textures for phase map and normal map");

  m_phaseMap0AttachPoint      = GL_COLOR_ATTACHMENT0_EXT;
  m_phaseMap1AttachPoint      = GL_COLOR_ATTACHMENT1_EXT;
  m_phaseMap2AttachPoint	  = GL_COLOR_ATTACHMENT2_EXT;
  m_depthMapAttachPoint       = GL_COLOR_ATTACHMENT3_EXT;
  m_normalMapAttachPoint      = GL_COLOR_ATTACHMENT4_EXT;
  m_referencePhaseAttachPoint = GL_COLOR_ATTACHMENT5_EXT;

  m_fringeImage1.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage2.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage3.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  m_fringeImage4.init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
  
  m_fringeImages[0][0] = &m_fringeImage1;
  m_fringeImages[0][1] = &m_fringeImage2; 
  m_fringeImages[1][0] = &m_fringeImage3;
  m_fringeImages[1][1] = &m_fringeImage4;

  m_phaseMap0.init        (width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
  m_phaseMap1.init        (width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
  m_phaseMap2.init        (width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
  m_depthMap.init         (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_normalMap.init        (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);
  m_referencePhase.init   (width, height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);

  m_imageProcessor.init(width, height);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap0, m_phaseMap0AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap1, m_phaseMap1AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap2, m_phaseMap2AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_depthMap, m_depthMapAttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_normalMap, m_normalMapAttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_referencePhase, m_referencePhaseAttachPoint);
  m_imageProcessor.unbind();

  OGLStatus::logOGLErrors("SixFringeCapture - initTextures()");
}

void SixFringeCapture::setGammaCutoff(float gamma)
  { m_gammaCutoff = gamma; }

void SixFringeCapture::setBlackLevel(float blackLevel)
  { m_blackLevel = blackLevel; }

void SixFringeCapture::setScalingFactor(float scalingFactor)
  { m_scalingFactor = scalingFactor; }

void SixFringeCapture::setShiftFactor(float shiftFactor)
  { m_shiftFactor = shiftFactor; }

Texture& SixFringeCapture::getDepthMap(void)
  { return m_depthMap; }

Texture& SixFringeCapture::getTextureMap(void)
  { return m_depthMap; } //  TODO - Put the texture map here

Texture& SixFringeCapture::getNormalMap(void)
  { return m_normalMap; }

void SixFringeCapture::decode(void)
{
  OGLStatus::logOGLErrors("SixFringeCapture - decode()");
  draw();
}

void SixFringeCapture::draw(void)
{
  if(m_captureReferencePhase)
  {
    //  If we dont have the reference phase then we are calculating it and we redraw
    m_imageProcessor.bind( );
    {
	  m_imageProcessor.bindDrawBuffer(m_phaseMap0AttachPoint);
	  m_phaseWrapper.bind();
	  m_fringeImages[m_frontBufferIndex][0]->bind(GL_TEXTURE0);
	  m_fringeImages[m_frontBufferIndex][1]->bind(GL_TEXTURE1); 
	  m_phaseWrapper.uniform("gammaCutoff", 0.0f);
	  m_imageProcessor.process( );

	  _gaussianFilter( m_phaseMap1AttachPoint, m_phaseMap2AttachPoint, m_phaseMap0, m_phaseMap1 );
	  _unwrapPhase( m_phaseMap1AttachPoint, m_phaseMap0, m_phaseMap2 );
	  _filterPhase( m_referencePhaseAttachPoint, m_phaseMap1 );
    }
    m_imageProcessor.unbind();

    m_haveReferencePhase = true;
    m_captureReferencePhase = false;
  }
  else if(m_haveReferencePhase)
  {
	m_imageProcessor.bind();
	{
      _wrapPhase( m_phaseMap0AttachPoint );
	  _gaussianFilter( m_phaseMap1AttachPoint, m_phaseMap2AttachPoint, m_phaseMap0, m_phaseMap1 );
	  _unwrapPhase( m_phaseMap1AttachPoint, m_phaseMap0, m_phaseMap2 );
	  _filterPhase( m_phaseMap2AttachPoint, m_phaseMap1 );
      _calculateDepthMap( m_phaseMap2 );
      _calculateNormalMap( );
	}
    m_imageProcessor.unbind();
  }

  m_fpsCalculator.frameUpdate();
  OGLStatus::logOGLErrors("SixFringeCapture - draw()");
}

int	SixFringeCapture::getFringeCount()
  { return 6; }

bool SixFringeCapture::newImage(IplImage* image)
{
  bool needRedraw = false;

  cvSetImageCOI(m_fringeLoadingImage.get(), (m_currentChannelLoad + 1));
  cvCopy(image, m_fringeLoadingImage.get());
  cvSetImageCOI(m_fringeLoadingImage.get(), 0);

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
	  { m_captureReferencePhase = true; }

    m_currentChannelLoad = 0;
    m_currentFringeLoad = 0;
    swapFringeBuffers();
    m_3dpsCalculator.frameUpdate();
    needRedraw = true;
  }

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("SixFringeCapture - setBackHoloBuffer()");

  return needRedraw;
}

void SixFringeCapture::swapFringeBuffers(void)
{
  //	Switch the front and back buffer
  m_frontBufferIndex = (m_frontBufferIndex + 1) % 2;

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("SixFringeCapture - swapBuffers()");
}

void SixFringeCapture::captureReferencePlane(void)
  { m_captureReferencePhase = true; }

void SixFringeCapture::loadReferencePlane(void* callbackInstance, shared_ptr<IplImage> (*imageLoaderFunction)(void* callbackInstance))
{
  shared_ptr<IplImage> fringe1 = imageLoaderFunction(callbackInstance);
  shared_ptr<IplImage> fringe2 = imageLoaderFunction(callbackInstance);

  //  If we didn't get our fringes just abort
  if( nullptr == fringe1 || nullptr == fringe2 )
	{ return; }

  //  Resize our input to match the loaded reference plane
  resizeInput(fringe1->width, fringe1->height);

  //  Signify that we want to capture the reference plane
  captureReferencePlane();

  int backBufferIndex = (m_frontBufferIndex + 1) % 2;
  m_fringeImages[backBufferIndex][0]->transferToTexture(fringe1.get());
  m_fringeImages[backBufferIndex][1]->transferToTexture(fringe2.get());

  swapFringeBuffers();
}

double SixFringeCapture::getFrameRate( void )
  { return m_fpsCalculator.getFrameRate(); }

double SixFringeCapture::get3DRate( void )
  { return m_3dpsCalculator.getFrameRate(); }

string SixFringeCapture::getCaptureName( void )
  { return "Six Fringe Capture"; }

void SixFringeCapture::_gaussianFilter( GLenum drawBuffer1, GLenum drawBuffer2, Texture& readBuffer1, Texture& readBuffer2 )
{
  m_imageProcessor.bindDrawBuffer( drawBuffer1 );
  m_gaussFilter.bind( );
  readBuffer1.bind( GL_TEXTURE0 );
  m_imageProcessor.process( );

  m_imageProcessor.bindDrawBuffer( drawBuffer2 );
  m_gaussFilter.flipFilter( );
  readBuffer2.bind( GL_TEXTURE0 );
  m_imageProcessor.process( );
}

void SixFringeCapture::_wrapPhase( GLenum drawBuffer )
{
  m_imageProcessor.bindDrawBuffer( drawBuffer );
  m_phaseWrapper.bind();
  //  Grab so that it doesn't change during a draw
  int frontBuffer = m_frontBufferIndex;
  m_fringeImages[frontBuffer][0]->bind(GL_TEXTURE0);
  m_fringeImages[frontBuffer][1]->bind(GL_TEXTURE1); 
  m_phaseWrapper.uniform( "gammaCutoff", m_gammaCutoff );
  m_phaseWrapper.uniform( "intensityCutoff", m_blackLevel );
  m_imageProcessor.process();
}

void SixFringeCapture::_unwrapPhase( GLenum drawBuffer, Texture& unfilteredPhase, Texture& filteredPhase )
{
  m_imageProcessor.bindDrawBuffer( drawBuffer );
  m_phaseUnwrapper.bind( );
  unfilteredPhase.bind( GL_TEXTURE0 );
  filteredPhase.bind( GL_TEXTURE1 );
  m_imageProcessor.process( );
}

void SixFringeCapture::_filterPhase( GLenum drawBuffer, Texture& readBuffer )
{
  m_imageProcessor.bindDrawBuffer( drawBuffer );
  m_phaseFilter.bind( );
  readBuffer.bind( GL_TEXTURE0 );
  m_imageProcessor.process( );
}

void SixFringeCapture::_calculateDepthMap( Texture& unwrappedPhase )
{
  m_imageProcessor.bindDrawBuffer(m_depthMapAttachPoint);
  m_depthCalculator.uniform("scalingFactor", m_scalingFactor);
  m_depthCalculator.bind();
  unwrappedPhase.bind(GL_TEXTURE0);
  m_referencePhase.bind(GL_TEXTURE1);
  m_imageProcessor.process();
}

void SixFringeCapture::_calculateNormalMap( )
{
  m_imageProcessor.bindDrawBuffer(m_normalMapAttachPoint);
  m_normalCalculator.bind();
  m_depthMap.bind(GL_TEXTURE0);
  m_imageProcessor.process();
}
