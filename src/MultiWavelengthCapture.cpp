#include "MultiWavelengthCapture.h"

MultiWavelengthCapture::MultiWavelengthCapture(void)
{
	m_hasBeenInit = false;
}

void MultiWavelengthCapture::init()
{
	if(!m_hasBeenInit)
	{
		initShaders();
                _initTextures(576, 576);
		_initLighting();
		
		m_controller.init(512, 512);
		m_camera.init(0.0f, 0.75f, 1.0f, 0.0f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f);
		m_camera.setMode(1);
		
		m_mesh = new TriMesh(512, 512);
		
		m_mesh->initMesh();
		m_hasBeenInit = true;
		
                m_fringeImages[0] = &m_fringeImage1;
                m_fringeImages[1] = &m_fringeImage2;
                m_fringeImages[2] = &m_fringeImage3;
	}
}

void MultiWavelengthCapture::initShaders(void)
{
	//	Create the shaders
        m_phaseCalculator.init("Shaders/MultiWavelength/PhaseCalculator.vert", "Shaders/MultiWavelength/PhaseCalculator.frag");
        m_phaseCalculator.uniform("fringe1", 0);
        m_phaseCalculator.uniform("fringe2", 1);
        m_phaseCalculator.uniform("fringe3", 2);

        m_phaseCalculator.bindAttributeLocation("vert", 0);
        m_phaseCalculator.bindAttributeLocation("vertTexCoord", 1);

        GLuint loc = glGetAttribLocation(m_phaseCalculator.shaderID(), "vert");
        GLuint loc1 = glGetAttribLocation(m_phaseCalculator.shaderID(), "vertTexCoord");

	m_phaseFilter.init("Shaders/MedianFilter3x3.vert", "Shaders/MedianFilter3x3.frag");
	m_phaseFilter.uniform("image", 0);
        m_phaseFilter.uniform("width", 576.0f);
        m_phaseFilter.uniform("height", 576.0f);

	m_normalCalculator.init("Shaders/NormalCalculator.vert", "Shaders/NormalCalculator.frag");
	m_normalCalculator.uniform("phaseA", 0);
        m_normalCalculator.uniform("width", 576.0f);
        m_normalCalculator.uniform("height", 576.0f);

	m_finalRender.init("Shaders/FinalRender.vert", "Shaders/FinalRender.frag");
	m_finalRender.uniform("normals", 0);
	m_finalRender.uniform("phaseMap", 1);
	m_finalRender.uniform("holoImage", 2);
        m_finalRender.uniform("width", 576.0f);
	
        OGLStatus::logOGLErrors("MultiWavelengthCapture - initShaders()");
}

void MultiWavelengthCapture::_initTextures(GLuint width, GLuint height)
{
        Logger::logDebug("MultiWavelengthCapture - initTextures(): Creating textures for phase map and normal map");
	
	m_imageProcessor.init();
	m_imageProcessor.unbind();
	
	m_phaseMap0AttachPoint = GL_COLOR_ATTACHMENT0_EXT;
	m_phaseMap1AttachPoint = GL_COLOR_ATTACHMENT1_EXT;
	m_normalMapAttachPoint = GL_COLOR_ATTACHMENT2_EXT;

        m_fringeImage1.init(width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
        m_fringeImage2.init(width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
        m_fringeImage3.init(width, height, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
	m_phaseMap0.init(width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
	m_phaseMap1.init(width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
	m_normalMap.init(width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
	
	m_imageProcessor.setTextureAttachPoint(m_phaseMap0, m_phaseMap0AttachPoint);
	m_imageProcessor.setTextureAttachPoint(m_phaseMap1, m_phaseMap1AttachPoint);
	m_imageProcessor.setTextureAttachPoint(m_normalMap, m_normalMapAttachPoint);

        OGLStatus::logOGLErrors("MultiWavelengthCapture - initTextures()");
}

void MultiWavelengthCapture::draw(void)
{
	m_imageProcessor.bind();
	{
		//	Pass 1
		m_imageProcessor.bindDrawBuffer(m_phaseMap0AttachPoint);
		m_phaseCalculator.bind();
		glActiveTexture(GL_TEXTURE0);
                m_fringeImage1.bind();
                glActiveTexture(GL_TEXTURE1);
                m_fringeImage2.bind();
                glActiveTexture(GL_TEXTURE2);
                m_fringeImage3.bind();
                m_imageProcessor.process();
		
		//	Pass 2
		m_imageProcessor.bindDrawBuffer(m_phaseMap1AttachPoint);
		m_phaseFilter.bind();
		glActiveTexture(GL_TEXTURE0);
		m_phaseMap0.bind();
		m_imageProcessor.process();
		
		//	Pass 3
		m_imageProcessor.bindDrawBuffer(m_normalMapAttachPoint);
		m_normalCalculator.bind();
		glActiveTexture(GL_TEXTURE0);
		m_phaseMap1.bind();
		m_imageProcessor.process();
	}
	m_imageProcessor.unbind();
	
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	
	m_camera.applyMatrix();
	
	glColor3f(0.0f, 1.0f, 0.0f);
		
	m_finalRender.bind();
	{
		glActiveTexture(GL_TEXTURE0);
		m_normalMap.bind();
		glActiveTexture(GL_TEXTURE1);
		m_phaseMap1.bind();
                glActiveTexture(GL_TEXTURE2);
                m_phaseMap1.bind();

		//	Draw a plane of pixels
		m_controller.applyTransform();
		m_mesh->draw();
	}
	m_finalRender.unbind();
	glPopMatrix();
	
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

void MultiWavelengthCapture::setBackBuffer(IplImage* image)
{
        //int backBufferIndex = (m_frontBufferIndex + 1) % 2;
        //m_holoImages[backBufferIndex]->transferToTexture(image);
	
	//	Make sure we dont have any errors
	OGLStatus::logOGLErrors("Holodecoder - setBackHoloBuffer()");
}

void MultiWavelengthCapture::swapBuffers(void)
{
	//	Switch the front and back buffer
	m_frontBufferIndex = (m_frontBufferIndex + 1) % 2;
	
	//	Make sure we dont have any errors
	OGLStatus::logOGLErrors("Holodecoder - swapBuffers()");
}

void MultiWavelengthCapture::loadTestData(void)
{
    //  Load the test data
    const string path("/home/karpinsn/Dropbox/Research/Data/MultiwaveLength/");

    ImageIO io;

    m_fringeImage1.transferToTexture(io.readImage(path + "fringe1.png"));
    m_fringeImage2.transferToTexture(io.readImage(path + "fringe2.png"));
    m_fringeImage3.transferToTexture(io.readImage(path + "fringe3.png"));
}

void MultiWavelengthCapture::_initLighting(void)
{
	GLfloat mat_specular[] = {.1f, .1f, .1f, .1f};
	GLfloat mat_shininess[] = {1.0f};
	GLfloat light_position[] = {2.0f, 2.0f, 4.0f, 1.0f};
	GLfloat white_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
	
	glClearColor(1.0, 1.0, 1.0, 0.0);
	
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
