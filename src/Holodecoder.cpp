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
		
		m_mesh = new PointCloudMesh(512, 512, 4);
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
	
	//	Bind the holoimage to texture 0
	m_decoderShader.bind();
	GLuint holoimageLocation = glGetUniformLocation(m_decoderShader.shaderID(), "holoImage");
	glUniform1i(holoimageLocation, 0);
	m_decoderShader.unbind();
}

void Holodecoder::initHoloBuffers(void)
{
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
}

void Holodecoder::draw(void)
{
	glPushMatrix();
	
	m_camera->applyMatrix();
	
	glColor3f(0.0f, 1.0f, 0.0f);
	
	m_decoderShader.bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_holoImageTex);
	
	//	Draw a plane of pixels
	m_controller->applyTransform();
	m_mesh->Draw();
	m_decoderShader.unbind();

	glPopMatrix();
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
	printGLErrors();
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
	printGLErrors();
}

void Holodecoder::printGLErrors(void)
{
	//	Print any errors if we have them
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
		cout << "OpenGL Error: " << gluErrorString(error) << endl;
    }
}