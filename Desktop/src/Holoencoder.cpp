#include "Holoencoder.h"

Holoencoder::Holoencoder(void)
{
	m_hasBeenInit = false;
        m_currentMesh = NULL;
}

void Holoencoder::init()
{	
  init(512, 512);
}

void Holoencoder::init(float width, float height)
{
  if(!m_hasBeenInit)
  {
      m_width = width;
      m_height = height;

      m_currentMesh = NULL;
      _initFBO();

      m_controller.init(m_width, m_height);
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
}

void Holoencoder::draw(void)
{	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
    {
      m_camera->applyMatrix();

      //	Draw the currentMesh
      glm::mat4 projectorModelView = m_projectorModelView;
      glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f),
                                         glm::vec3(4.8f));
      glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(-.1, -.2, .1));

      m_controller.applyTransform();
      projectorModelView = projectorModelView * m_controller.getTransform() * scaleMatrix * translate;

      glScalef(4.8, 4.8, 4.8);
      glTranslatef(-.1, -.2, .1);

      m_encoderShader.bind();
      m_encoderShader.uniform("projectorModelView", projectorModelView);

      glColor3f(.8, .8, .8);

      if(NULL != m_currentMesh)
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
    m_width = 512;
    m_height = 512;
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}


void Holoencoder::cameraSelectMode(int mode)
{
	m_camera->setMode(mode);
}

void Holoencoder::mousePressEvent(int mouseX, int mouseY)
{	
	m_controller.mousePressEvent(mouseX, mouseY);
}

void Holoencoder::mouseMoveEvent(int mouseX, int mouseY)
{	
	m_controller.mouseMoveEvent(mouseX, mouseY);
}

void Holoencoder::setCurrentMesh(AbstractMesh* current)
{
	if(NULL != m_currentMesh)
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
  OGLStatus::logOGLErrors("MultiWavelengthCapture - decode()");
  
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_holoimageFBO);
  draw();
  glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

MeshInterchange* Holoencoder::getEncodedData()
{
  return new MeshInterchange(m_holoimage);
}