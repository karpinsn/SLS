#include "Holodecoder.h"

Holodecoder::Holodecoder(OpenGLWidget* glContext)
{
	m_glContext = glContext;
	m_controller = new Arcball(512, 512);
	m_hasBeenInit = false;
	DATA_SIZE = 512 * 512 * 4;
}

void Holodecoder::init()
{
	if(!m_hasBeenInit)
	{
		initShaders();
		initHoloBuffers();
		_initTextures(512, 512);
		_initLighting();
		
		//m_mesh = new PointCloudMesh(512, 512, 4);
		m_mesh = new TriMesh(512, 512);
		m_camera = new Camera();
		m_camera->init(2.0f, 2.0f, -2.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f);
	
		m_mesh->initMesh();
		haveHoloImage = false;
		m_hasBeenInit = true;
	}
}

void Holodecoder::initShaders(void)
{
	//	Create the shaders
	m_decoderShader.init("Shaders/Holodecoder.vert", "Shaders/Holodecoder.frag");
	m_decoderShader.uniform("holoImage", 0);
	
	m_phaseCalculator.init("Shaders/PhaseCalculator.vert", "Shaders/PhaseCalculator.frag");
	m_phaseCalculator.uniform("holoImage", 0);
	
	m_phaseFilter.init("Shaders/MedianFilter3x3.vert", "Shaders/MedianFilter3x3.frag");
	m_phaseFilter.uniform("image", 0);
	m_phaseFilter.uniform("width", 512.0f);
	m_phaseFilter.uniform("height", 512.0f);
	
	m_normalCalculator.init("Shaders/NormalCalculator.vert", "Shaders/NormalCalculator.frag");
	m_normalCalculator.uniform("phaseA", 0);
	
	m_finalRender.init("Shaders/FinalRender.vert", "Shaders/FinalRender.frag");
	m_finalRender.uniform("normals", 0);
	m_finalRender.uniform("holoImage", 1);
	
	OGLStatus::logOGLErrors("Holodecoder - initShaders()");
}

void Holodecoder::initHoloBuffers(void)
{
	Logger::logDebug("Holodecoder - initHoloBuffers(): Creating Holo textures and PBOs");
	
	//	Create the holoimage texture
	glGenTextures(1, &m_holoImageTex);
	glBindTexture(GL_TEXTURE_2D, m_holoImageTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//	Need to init the PBOs for the holoiamge
	//	create 2 pixel buffer objects, you need to delete them when program exits.
	//	glBufferDataARB with NULL pointer reserves only memory space.
	glGenBuffersARB(2, m_bufferIds);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, m_bufferIds[0]);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, m_bufferIds[1]);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	
	OGLStatus::logOGLErrors("Holodecoder - initHoloBuffers()");
}

void Holodecoder::_initTextures(GLuint width, GLuint height)
{
	Logger::logDebug("Holodecoder - initTextures(): Creating textures for phase map and normal map");
	
	m_imageProcessor.init();
	m_imageProcessor.unbind();
	
	m_phaseMap0AttachPoint = GL_COLOR_ATTACHMENT0_EXT;
	m_phaseMap1AttachPoint = GL_COLOR_ATTACHMENT1_EXT;
	m_normalMapAttachPoint = GL_COLOR_ATTACHMENT2_EXT;
	
	m_phaseMap0.init(width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
	m_phaseMap1.init(width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
	m_normalMap.init(width, height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);
	
	m_imageProcessor.setTextureAttachPoint(m_phaseMap0, m_phaseMap0AttachPoint);
	m_imageProcessor.setTextureAttachPoint(m_phaseMap1, m_phaseMap1AttachPoint);
	m_imageProcessor.setTextureAttachPoint(m_normalMap, m_normalMapAttachPoint);

	OGLStatus::logOGLErrors("Holodecoder - initTextures()");
}

void Holodecoder::draw(void)
{
	m_imageProcessor.bind();
	{
		//	Pass 1
		m_imageProcessor.bindDrawBuffer(m_phaseMap0AttachPoint);
		m_phaseCalculator.bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_holoImageTex);
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
	
	glPushMatrix();
	
	m_camera->applyMatrix();
	
	glColor3f(0.0f, 1.0f, 0.0f);
	
	m_finalRender.bind();
	{
		glActiveTexture(GL_TEXTURE0);
		m_normalMap.bind();
		glActiveTexture(GL_TEXTURE1);
		m_phaseMap1.bind();
		
		//	Draw a plane of pixels
		m_controller->applyTransform();
		m_mesh->Draw();
	}
	m_finalRender.unbind();

	glPopMatrix();
	
	OGLStatus::logOGLErrors("Holodecoder - draw()");
}

void Holodecoder::resize(int width, int height)
{
	//m_camera->reshape(width, height);
	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
}

void Holodecoder::mousePressEvent(int mouseX, int mouseY)
{
	//m_camera->mousePressed(mouseX, mouseY);
	m_controller->mousePressEvent(mouseX, mouseY);
}

void Holodecoder::mouseMoveEvent(int mouseX, int mouseY)
{
	//m_camera->mouseMotion(mouseX, mouseY);
	m_controller->mouseMoveEvent(mouseX, mouseY);
}

void Holodecoder::setHoloImage(GLuint holoImageTex)
{
	m_holoImageTex = holoImageTex;
}

void Holodecoder::setBackHoloBuffer(IplImage* image)
{
	int backBufferIndex = (m_frontBufferIndex + 1) % 2;

	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, m_bufferIds[backBufferIndex]);
	glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, DATA_SIZE, 0, GL_STREAM_DRAW_ARB);
	GLubyte* ptr = (GLubyte*)glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
	
	if(ptr)
	{
		// update data directly on the mapped buffer
		IplImage *imageRGBA = cvCreateImage(cvSize(512, 512), IPL_DEPTH_8U, 4);

		cvSetImageCOI(image, 1);
		cvSetImageCOI(imageRGBA, 1);
		cvCopy(image, imageRGBA);

		cvSetImageCOI(image, 2);
		cvSetImageCOI(imageRGBA, 2);
		cvCopy(image, imageRGBA);
		
		cvSetImageCOI(image, 3);
		cvSetImageCOI(imageRGBA, 3);
		cvCopy(image, imageRGBA);
		
		cvSetImageCOI(imageRGBA, 0);
		cvSetImageCOI(image, 0);
		
		memcpy(ptr, imageRGBA->imageData, DATA_SIZE);
		
		cvReleaseImage(&imageRGBA);
		
		glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB); // release pointer to mapping buffer
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	}
	
	//	Make sure we dont have any errors
	OGLStatus::logOGLErrors("Holodecoder - setBackHoloBuffer()");
}

void Holodecoder::swapBuffers(void)
{
	//	Switch the front and back buffer
	m_frontBufferIndex = (m_frontBufferIndex + 1) % 2;
	
	//	Bind the front buffer
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, m_bufferIds[m_frontBufferIndex]);
	
	//	Copy the front PBO to the texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_holoImageTex);	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 512, 512, GL_BGRA, GL_UNSIGNED_BYTE, 0);
	
	//	Make sure we dont have any errors
	OGLStatus::logOGLErrors("Holodecoder - swapBuffers()");
}

void Holodecoder::_initLighting(void)
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