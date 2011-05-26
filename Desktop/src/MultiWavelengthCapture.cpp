#include "MultiWavelengthCapture.h"

MultiWavelengthCapture::MultiWavelengthCapture(void)
{
  m_hasBeenInit = false;
  m_haveReferencePhase = false;
  m_captureReferencePhase = false;
  m_currentFringeLoad = 0;
  m_currentChannelLoad = 0;
  m_frontBufferIndex = 0;
}

void MultiWavelengthCapture::init()
{
  if(!m_hasBeenInit)
  {
    _initShaders(640, 480);
    _initTextures(640, 480);

    m_axis.init();

    m_controller.init(512, 512);
    m_camera.init(0.0f, 0.75f, 1.0f, 0.0f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f);
    m_camera.setMode(1);

    m_mesh = new TriMesh(640, 480);

    m_mesh->initMesh();

    m_fringeLoadingImage = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);

    m_hasBeenInit = true;
  }
}

void MultiWavelengthCapture::_initShaders(float width, float height)
{
  // Create the shaders
  m_phaseCalculator.init();
  m_phaseCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/MultiWavelength/PhaseCalculator.vert"));
  m_phaseCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/MultiWavelength/PhaseCalculator.frag"));
  m_phaseCalculator.bindAttributeLocation("vert", 0);
  m_phaseCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_phaseCalculator.link();
  m_phaseCalculator.uniform("fringeImage1", 0);
  m_phaseCalculator.uniform("fringeImage2", 1);
  m_phaseCalculator.uniform("fringeImage3", 2);

  m_phaseFilter.init();
  m_phaseFilter.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/MedianFilter3x3.vert"));
  m_phaseFilter.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/MedianFilter3x3.frag"));
  m_phaseCalculator.bindAttributeLocation("vert", 0);
  m_phaseCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_phaseFilter.link();
  m_phaseFilter.uniform("image", 0);
  m_phaseFilter.uniform("width", width);
  m_phaseFilter.uniform("height", height);

  m_normalCalculator.init();
  m_normalCalculator.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/NormalCalculator.vert"));
  m_normalCalculator.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/NormalCalculator.frag"));
  m_phaseCalculator.bindAttributeLocation("vert", 0);
  m_phaseCalculator.bindAttributeLocation("vertTexCoord", 1);

  m_normalCalculator.link();
  m_normalCalculator.uniform("phaseA", 0);
  m_normalCalculator.uniform("width", width);
  m_normalCalculator.uniform("height", height);

  m_finalRender.init();
  m_finalRender.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/MultiWavelength/FinalRender.vert"));
  m_finalRender.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/MultiWavelength/FinalRender.frag"));
  m_finalRender.link();
  m_finalRender.uniform("normals", 0);
  m_finalRender.uniform("phaseMap", 1);
  m_finalRender.uniform("referencePhaseMap", 2);

  OGLStatus::logOGLErrors("MultiWavelengthCapture - initShaders()");
}

void MultiWavelengthCapture::_initTextures(GLuint width, GLuint height)
{
  Logger::logDebug("MultiWavelengthCapture - initTextures(): Creating textures for phase map and normal map");

  m_phaseMap0AttachPoint      = GL_COLOR_ATTACHMENT0_EXT;
  m_phaseMap1AttachPoint      = GL_COLOR_ATTACHMENT1_EXT;
  m_normalMapAttachPoint      = GL_COLOR_ATTACHMENT2_EXT;
  m_referencePhaseAttachPoint = GL_COLOR_ATTACHMENT3_EXT;

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

  m_phaseMap0.init        (width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
  m_phaseMap1.init        (width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
  m_normalMap.init        (width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
  m_referencePhase.init   (width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);

  m_imageProcessor.init(width, height);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap0, m_phaseMap0AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_phaseMap1, m_phaseMap1AttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_normalMap, m_normalMapAttachPoint);
  m_imageProcessor.setTextureAttachPoint(m_referencePhase, m_referencePhaseAttachPoint);
  m_imageProcessor.unbind();

  OGLStatus::logOGLErrors("MultiWavelengthCapture - initTextures()");
}

void MultiWavelengthCapture::draw(void)
{
  if(m_captureReferencePhase)
  {
    //  If we dont have the reference phase then we are calculating it and we redraw
    m_imageProcessor.bind();
    {
      m_imageProcessor.bindDrawBuffer(m_referencePhaseAttachPoint);
      m_phaseCalculator.bind();
      m_fringeImages[m_frontBufferIndex][m_currentFringeLoad]->bind(GL_TEXTURE0);
      m_fringeImages[m_frontBufferIndex][m_currentFringeLoad]->bind(GL_TEXTURE1);
      m_fringeImages[m_frontBufferIndex][m_currentFringeLoad]->bind(GL_TEXTURE2);
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
      //	Pass 1
      m_imageProcessor.bindDrawBuffer(m_phaseMap0AttachPoint);
      m_phaseCalculator.bind();
      m_fringeImages[m_frontBufferIndex][m_currentFringeLoad]->bind(GL_TEXTURE0);
      m_fringeImages[m_frontBufferIndex][m_currentFringeLoad]->bind(GL_TEXTURE1);
      m_fringeImages[m_frontBufferIndex][m_currentFringeLoad]->bind(GL_TEXTURE2);
      m_imageProcessor.process();

      //	Pass 2
      m_imageProcessor.bindDrawBuffer(m_phaseMap1AttachPoint);
      m_phaseFilter.bind();
      m_phaseMap0.bind(GL_TEXTURE0);
      m_imageProcessor.process();

      //	Pass 3
      m_imageProcessor.bindDrawBuffer(m_normalMapAttachPoint);
      m_normalCalculator.bind();
      m_phaseMap1.bind(GL_TEXTURE0);
      m_imageProcessor.process();
	}
	m_imageProcessor.unbind();
	
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	
	m_camera.applyMatrix();
    m_controller.applyTransform();

	glColor3f(0.0f, 1.0f, 0.0f);

    glm::mat4 mvMatrix;
    glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(mvMatrix));
    m_axis.draw(mvMatrix);

	m_finalRender.bind();
	{
      m_normalMap.bind(GL_TEXTURE0);
      m_phaseMap1.bind(GL_TEXTURE1);
      m_referencePhase.bind(GL_TEXTURE2);

      // Draw a plane of pixels
      m_mesh->draw();
	}
	m_finalRender.unbind();

	glPopMatrix();
  }
  OGLStatus::logOGLErrors("MultiWavelengthCapture - draw()");
}

void MultiWavelengthCapture::resize(int width, int height)
{
  m_camera.reshape(width, height);
  gluPerspective(45.0, 1.0, .00001, 10.0);
}

void MultiWavelengthCapture::cameraSelectMode(int mode)
{
  m_camera.setMode(mode);
}

void MultiWavelengthCapture::mousePressEvent(int mouseX, int mouseY)
{
  m_camera.mousePressed(mouseX, mouseY);
}

void MultiWavelengthCapture::mouseMoveEvent(int mouseX, int mouseY)
{
  m_camera.mouseMotion(mouseX, mouseY);
}

void MultiWavelengthCapture::newImage(IplImage* image)
{
  cvSetImageCOI(m_fringeLoadingImage, (m_currentChannelLoad + 1));
  cvCopy(image, m_fringeLoadingImage);
  cvSetImageCOI(m_fringeLoadingImage, 0);

  m_currentChannelLoad++;

  if(m_currentChannelLoad == 3)
  {
    int backBufferIndex = (m_frontBufferIndex + 1) % 2;
    m_fringeImages[backBufferIndex][m_currentFringeLoad]->transferToTexture(m_fringeLoadingImage);
    m_currentChannelLoad = 0;
    m_currentFringeLoad++;
  }

  if(m_currentFringeLoad == 3)
  {
    m_currentChannelLoad = 0;
    m_currentFringeLoad = 0;
    swapBuffers();
  }

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("MultiWavelengthCapture - setBackHoloBuffer()");
}

void MultiWavelengthCapture::swapBuffers(void)
{
  //	Switch the front and back buffer
  m_frontBufferIndex = (m_frontBufferIndex + 1) % 2;

  //	Make sure we dont have any errors
  OGLStatus::logOGLErrors("MultiWavelengthCapture - swapBuffers()");
}

void MultiWavelengthCapture::captureReferencePlane(void)
{
  m_captureReferencePhase = true;
}

void MultiWavelengthCapture::loadTestData(void)
{
  /*
  if(m_haveReferencePhase)
  {
    //  Load the test data
    const string path("/home/karpinsn/Desktop/DeereParticleTrack/4-21-2011/Cotton/");

    ImageIO io;

    m_fringeImage1.transferToTexture(io.readImage(path + "1.png"));
    m_fringeImage2.transferToTexture(io.readImage(path + "2.png"));
    m_fringeImage3.transferToTexture(io.readImage(path + "3.png"));
  }
  else
  {
    //  Load the test data
    const string path("/home/karpinsn/Desktop/DeereParticleTrack/4-21-2011/Cotton/Reference/");

    ImageIO io;

    m_fringeImage1.transferToTexture(io.readImage(path + "1.png"));
    m_fringeImage2.transferToTexture(io.readImage(path + "2.png"));
    m_fringeImage3.transferToTexture(io.readImage(path + "3.png"));
  }
  */
}
