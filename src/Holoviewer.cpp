#include "Holoviewer.h"

void Holoviewer::init()
{
	m_mesh = new PointCloudMesh(512, 512, 4);
	m_decoderShader.init("Shaders/Holodecoder.vert", "Shaders/Holodecoder.frag");
	
	//	Generate a blank texture since setHoloImage deletes a texture
	glGenTextures(1, &m_holoImageTex);
	unsigned char* tex = new unsigned char[512*512*3];
	setHoloImage(tex);
	delete tex;
	
	m_camera = new Camera();
	m_camera->init(0.0f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f);
	
	m_mesh->initMesh();
	haveHoloImage = false;
}

void Holoviewer::draw(void)
{
	m_camera->applyMatrix();
	
	glColor3f(0.0f, 1.0f, 0.0f);
	
	m_decoderShader.bind();
	
	GLuint holoimageLocation = glGetUniformLocation(m_decoderShader.shaderID(), "holoImage");
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_holoImageTex);
	glUniform1i(holoimageLocation, 0);
	
	//	Draw a plane of pixels
	m_mesh->Draw();
	m_decoderShader.unbind();
}

void Holoviewer::resize(int width, int height)
{
	m_camera->reshape(width, height);
	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
}

void Holoviewer::mousePressEvent(int mouseX, int mouseY)
{
	m_camera->mousePressed(mouseX, mouseY);
}

void Holoviewer::mouseMoveEvent(int mouseX, int mouseY)
{
	m_camera->mouseMotion(mouseX, mouseY);
}

#include "ImageIO.h"

void Holoviewer::setHoloImage(unsigned char* textureData)
{
	glDeleteTextures(1, &m_holoImageTex);
	ImageIO io;
	
	io.loadOpenGLTexture2D("/Users/Karpinsn/Workspace/3DMVLab/HoloimageViewer/trunk", &m_holoImageTex, IL_RGB);
	
	//glGenTextures(1, &m_holoImageTex);
	//glBindTexture(GL_TEXTURE_2D, m_holoImageTex);
	
	//	Use linear interpolation and then generate the mipmap
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, textureData);
}