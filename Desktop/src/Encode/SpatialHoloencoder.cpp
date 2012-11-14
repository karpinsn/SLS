#include "SpatialHoloencoder.h"

SpatialHoloencoder::SpatialHoloencoder(void)
{
	m_draw2Holoimage = false;
	m_hasBeenInit = false;
    m_currentData = nullptr;
	m_depthAttachPoint = GL_COLOR_ATTACHMENT0;
	m_holoimageAttachPoint = GL_COLOR_ATTACHMENT1;
}

void SpatialHoloencoder::init()
{	
  init(512, 512);
}

void SpatialHoloencoder::init(int width, int height)
{
  m_width = width;
  m_height = height;
  m_currentData = nullptr;

  if(!m_hasBeenInit)
  {
	  m_holoimage.init(m_width, m_height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	  m_depthMap.init(m_width, m_height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);

	  m_offscreenFBO.init(m_width, m_height);
	  m_offscreenFBO.setTextureAttachPoint(m_depthMap, m_depthAttachPoint);
	  m_offscreenFBO.setTextureAttachPoint(m_holoimage, m_holoimageAttachPoint);
	  m_offscreenFBO.unbind();

	  m_controller.init(0.0,0.0,0.0,.5);
      _initShaders();

      m_camera = unique_ptr<Camera>(new Camera());
      m_camera->init(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
      m_camera->setMode(4);

	  //	Define the projector projection matrix
      m_projectorModelView = glm::mat4();
      m_projectorModelView = m_projectorModelView * glm::rotate(30.0f, 0.0f, 1.0f, 0.0f);

      m_hasBeenInit = true;
  }
  else
  {
	  m_holoimage.reinit(m_width, m_height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	  m_depthMap.reinit(m_width, m_height, GL_RGB32F_ARB, GL_RGB, GL_FLOAT);

      m_offscreenFBO.reinit(m_width, m_height);
	  m_offscreenFBO.setTextureAttachPoint(m_depthMap, m_depthAttachPoint);
	  m_offscreenFBO.setTextureAttachPoint(m_holoimage, m_holoimageAttachPoint);
	  m_offscreenFBO.unbind();
  }
}

void SpatialHoloencoder::_initShaders(void)
{
    m_encoderShader.init();
    m_encoderShader.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/SpatialHoloencoder/Vert2Holo.vert"));
    m_encoderShader.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/SpatialHoloencoder/Vert2Holo.frag"));
    m_encoderShader.link();
    m_encoderShader.uniform("fringeFrequency", 16.0f);
}

void SpatialHoloencoder::draw(void)
{	
	if(nullptr != m_currentData)
	{
		glPushMatrix(); 
		{
		  m_camera->applyMatrix();
		  m_controller.applyTransform();

		  //  Fetch the modelview, set the transforms, then put it back
		  glm::mat4 modelView;
		  glGetFloatv(GL_MODELVIEW_MATRIX , glm::value_ptr(modelView));
		  modelView = modelView * m_scale * m_translate;
		  glLoadMatrixf(glm::value_ptr(modelView));

		  glm::mat4 projectorModelView = m_projectorModelView * m_controller.getTransform() * m_scale * m_translate;;

		  //	Now do the actual encoding
		  if(m_draw2Holoimage)
		  {
			m_offscreenFBO.bind();
			m_offscreenFBO.bindDrawBuffer(m_holoimageAttachPoint);
		  }

		  m_encoderShader.bind();
		  m_encoderShader.uniform("projectorModelView", projectorModelView);

		  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		  if(nullptr != m_currentData)
		  {
			m_currentData->getMesh()->draw();
		  }
		  m_encoderShader.unbind();

		  if(m_draw2Holoimage)
		  {
			  m_offscreenFBO.unbind();
		  }
		}
		glPopMatrix();
	}
}

void SpatialHoloencoder::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

void SpatialHoloencoder::cameraSelectMode(int mode)
{
	m_camera->setMode(mode);
}

void SpatialHoloencoder::mousePressEvent(int mouseX, int mouseY)
{	
	m_controller.mousePressEvent(m_camera->getMatrix(), mouseX, mouseY);
}

void SpatialHoloencoder::mouseMoveEvent(int mouseX, int mouseY)
{	
	m_controller.mouseDragEvent(m_camera->getMatrix(), mouseX, mouseY);
}

void SpatialHoloencoder::setCurrentMesh(shared_ptr<MeshInterchange> current)
{
  m_currentData = current;
}

void SpatialHoloencoder::encode(void)
{
  OGLStatus::logOGLErrors("SpatialHoloencoder - encoder()");
  
  m_draw2Holoimage = true;
  draw();
  m_draw2Holoimage = false;
}

MeshInterchange* SpatialHoloencoder::getEncodedData()
{
  return new MeshInterchange(&m_holoimage, false);
}

void SpatialHoloencoder::autoFitTransforms(void)
{
  if(nullptr != m_currentData && m_currentData->getPreferedFormat() == MeshInterchange::VERTEX_FORMAT)
  {
	m_translate = glm::translate(glm::mat4(1.0), -m_currentData->getMesh()->getBoundingBox().center);
	
	//	Calculate the scaling factor
	glm::vec4 min = glm::vec4(m_currentData->getMesh()->getBoundingBox().min, 1.0);
	glm::vec4 max = glm::vec4(m_currentData->getMesh()->getBoundingBox().max, 1.0);
	//	Translate the min and maxz
	min = m_translate * min;
	max = m_translate * max;
	//	Figure out what is the absolute maximum extent
	float scaleFactor = glm::max(glm::abs(min.x), glm::abs(max.x));
	scaleFactor = glm::max(glm::max(glm::abs(min.y), glm::abs(max.y)), scaleFactor);
	scaleFactor = glm::max(glm::max(glm::abs(min.z), glm::abs(max.z)), scaleFactor);

	//	Scale is 1 / scaleFactor since we are in a 0 to 1 Ortho projection
	m_scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f / scaleFactor));
  }
}
