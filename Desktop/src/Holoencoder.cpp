#include "Holoencoder.h"

Holoencoder::Holoencoder(void)
{
	m_hasBeenInit = false;
        m_currentMesh = nullptr;
}

void Holoencoder::init()
{	
  init(512, 512);
}

void Holoencoder::init(float width, float height)
{
  //	TODO - Comeback and fix this. Need to be able to reinit
  if(!m_hasBeenInit)
  {
      m_width = width;
      m_height = height;

      m_currentMesh = nullptr;
      _initFBO();

      //m_controller.init(m_width, m_height);
	  m_controller.init(0.0,0.0,0.0,.5);
      _initShaders();

      m_camera = new Camera();
      m_camera->init(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
      m_camera->setMode(4);

      //	Define the projector projection matrix
      m_projectorModelView = glm::mat4();
      m_projectorModelView = m_projectorModelView * glm::gtx::transform::rotate(30.0f, 0.0f, 1.0f, 0.0f);

      m_hasBeenInit = true;
  }
}

void Holoencoder::_initFBO()
{
    m_holoimage.init(m_width, m_height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);

	//	Need to create a render buffer object for the depth buffer
	glGenRenderbuffersEXT(1, &m_holoimageRBO);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_holoimageRBO);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_width, m_height);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	
	//	Create a framebuffer object
	glGenFramebuffersEXT(1, &m_holoimageFBO);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_holoimageFBO);
	
	//	Attach the texture to the FBO color attachment point
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_holoimage.getTextureId(), 0);
	
	//	Attach the renderbuffer to the depth attachment point
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_holoimageRBO);
	
	//	Check the FBO Status and make sure that it is complete
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		clog << "Holoimage FBO not correctly created" << endl;
	}
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void Holoencoder::_initShaders(void)
{
    m_encoderShader.init();
    m_encoderShader.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/Holoencoder.vert"));
    m_encoderShader.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/Holoencoder.frag"));
    m_encoderShader.link();

    m_encoderShader.uniform("fringeFrequency", 6.0f);

	float thresholdMap[64] = {0, 32, 8, 40, 2, 34, 10, 42,
							  48, 16, 56, 24, 50, 18, 58, 26,
							  12, 44, 4, 36, 14, 46, 6, 38,
							  60, 28, 52, 20, 62, 30, 54, 22,
							  3, 35, 11, 43, 1, 33, 9, 41,
							  51, 19, 59, 27, 49, 17, 57, 25,
							  15, 47, 7, 39, 13, 45, 5, 37,
							  63, 31, 55, 23, 61, 29, 53, 21};

	for(int i = 0; i < 64; ++i)
	{
		thresholdMap[i] *= 1.0/256.0;
	}

	m_encoderShader.uniform("thresholdMap", thresholdMap, 64);
}

void Holoencoder::draw(void)
{	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
    {
      m_camera->applyMatrix();

      //	Draw the currentMesh
      glm::mat4 projectorModelView = m_projectorModelView;
      
      m_controller.applyTransform();
      projectorModelView = projectorModelView * m_controller.getTransform() * m_scale * m_translate;

	  //  Fetch the modelview, set the transforms, then put it back
	  glm::mat4 modelView;
	  glGetFloatv(GL_MODELVIEW_MATRIX , glm::value_ptr(modelView));
	  modelView = modelView * m_scale * m_translate;
	  glLoadMatrixf(glm::value_ptr(modelView));

      m_encoderShader.bind();
      m_encoderShader.uniform("projectorModelView", projectorModelView);

      glColor3f(.8, .8, .8);

      if(nullptr != m_currentMesh)
      {
          m_currentMesh->draw();
      }

      m_encoderShader.unbind();
    }
	glPopMatrix();
}

Texture& Holoencoder::encodeOldWay()
{	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_holoimageFBO);
	
	draw();
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	return m_holoimage;
}

void Holoencoder::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

void Holoencoder::cameraSelectMode(int mode)
{
	m_camera->setMode(mode);
}

void Holoencoder::mousePressEvent(int mouseX, int mouseY)
{	
	m_controller.mousePressEvent(m_camera->getMatrix(), mouseX, mouseY);
}

void Holoencoder::mouseMoveEvent(int mouseX, int mouseY)
{	
	m_controller.mouseDragEvent(m_camera->getMatrix(), mouseX, mouseY);
}

void Holoencoder::setCurrentMesh(AbstractMesh* current)
{
	if(nullptr != m_currentMesh)
	{
		//	Make sure to delete the current mesh if its not null
		delete m_currentMesh;
	}
	
	m_currentMesh = current;
}

void Holoencoder::setCurrentMesh(MeshInterchange* current)
{
  m_currentMesh = current->getMesh();
}

void Holoencoder::encode(void)
{
  OGLStatus::logOGLErrors("Holoencoder - encoder()");
  
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_holoimageFBO);
  draw();
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

MeshInterchange* Holoencoder::getEncodedData()
{
  return new MeshInterchange(m_holoimage);
}

void Holoencoder::autoFitTransforms(void)
{
  if(nullptr != m_currentMesh)
  {
	m_translate = glm::translate(glm::mat4(1.0), -m_currentMesh->getBoundingBox().center);
	
	//	Calculate the scaling factor
	glm::vec4 min = glm::vec4(m_currentMesh->getBoundingBox().min, 1.0);
	glm::vec4 max = glm::vec4(m_currentMesh->getBoundingBox().max, 1.0);
	//	Translate the min and max
	min = m_translate * min;
	max = m_translate * max;
	//	Figure out what is the absolute maximum extent
	float scaleFactor = glm::max(glm::abs(min.x), glm::abs(max.x));
	scaleFactor = glm::max(glm::max(glm::abs(min.y), glm::abs(max.y)), scaleFactor);
	scaleFactor = glm::max(glm::max(glm::abs(min.z), glm::abs(max.z)), scaleFactor);
	//	Scale is just 1 / scaleFactor since we are in a -1 to 1 Ortho projection
	m_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f/scaleFactor));
  }
}