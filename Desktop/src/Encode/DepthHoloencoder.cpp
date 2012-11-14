#include "DepthHoloencoder.h"

DepthHoloencoder::DepthHoloencoder(void)
{
	m_draw2Holoimage = false;
	m_hasBeenInit = false;
    m_currentData = nullptr;
	m_depthAttachPoint = GL_COLOR_ATTACHMENT0;
	m_holoimageAttachPoint = GL_COLOR_ATTACHMENT1;
}

void DepthHoloencoder::init()
{	
  init(512, 512);
}

void DepthHoloencoder::init(int width, int height)
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

void DepthHoloencoder::_initShaders(void)
{
	m_depthShader.init();
	m_depthShader.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/DepthHoloencoder/Mesh2Depth.vert"));
	m_depthShader.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/DepthHoloencoder/Mesh2Depth.frag"));
	m_depthShader.link();

	m_depth2HoloShader.init();
	m_depth2HoloShader.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/DepthHoloencoder/Depth2Holo.vert"));
	m_depth2HoloShader.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/DepthHoloencoder/Depth2Holo.frag"));
	m_depth2HoloShader.link();
	m_depth2HoloShader.uniform("fringeFrequency", 16.0f);
	m_depth2HoloShader.uniform("depthMap", 0);
}

void DepthHoloencoder::draw(void)
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

		  //	Need to get our data into a depth format
		  if(m_currentData->getPreferedFormat() == MeshInterchange::VERTEX_FORMAT)
		  {
			  //	DepthMap
			  m_offscreenFBO.bind();
			  m_offscreenFBO.bindDrawBuffer(m_depthAttachPoint);
			  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			  m_depthShader.bind();
			  if(nullptr != m_currentData)
			  {
				m_currentData->getMesh()->draw();
			  }
			  m_offscreenFBO.unbind();

			  m_depth2HoloShader.bind();
			  m_depthMap.bind(GL_TEXTURE0);
		  }
		  else if(m_currentData->getPreferedFormat() == MeshInterchange::IMAGE_FORMAT)
		  {
			  m_depthMap.transferToTexture(m_currentData->getIplImage());
			  m_depth2HoloShader.bind();
			  m_depthMap.bind(GL_TEXTURE0);
		  }
		  else
		  {
			  m_depth2HoloShader.bind();
			  //	We have a depth map, just bind and use it
			  m_currentData->getTexture()->bind(GL_TEXTURE0);
		  }

		  //	Now do the actual encoding
		  if(m_draw2Holoimage)
		  {
			m_offscreenFBO.bind();
			m_offscreenFBO.bindDrawBuffer(m_holoimageAttachPoint);
		  }

		  m_offscreenFBO.process();

		  if(m_draw2Holoimage)
		  {
			  m_offscreenFBO.unbind();
		  }

		  m_depth2HoloShader.unbind();
		}
		glPopMatrix();
	}
}

void DepthHoloencoder::resize(int width, int height)
{
    m_width = width;
    m_height = height;
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

void DepthHoloencoder::cameraSelectMode(int mode)
{
	m_camera->setMode(mode);
}

void DepthHoloencoder::mousePressEvent(int mouseX, int mouseY)
{	
	m_controller.mousePressEvent(m_camera->getMatrix(), mouseX, mouseY);
}

void DepthHoloencoder::mouseMoveEvent(int mouseX, int mouseY)
{	
	m_controller.mouseDragEvent(m_camera->getMatrix(), mouseX, mouseY);
}

void DepthHoloencoder::setCurrentMesh(shared_ptr<MeshInterchange> current)
{
  m_currentData = current;
}

void DepthHoloencoder::encode(void)
{
  OGLStatus::logOGLErrors("DepthHoloencoder - encoder()");
  
  m_draw2Holoimage = true;
  draw();
  m_draw2Holoimage = false;
}

MeshInterchange* DepthHoloencoder::getEncodedData()
{
  return new MeshInterchange(&m_holoimage, false);
}

void DepthHoloencoder::autoFitTransforms(void)
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
