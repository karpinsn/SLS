#include "Holodecoder.h"

Holodecoder::Holodecoder(void)
{
	m_hasBeenInit = false;
}

void Holodecoder::init()
{
	if(!m_hasBeenInit)
	{
		m_mesh = new PointCloudMesh(512, 512, 4);
		m_decoderShader.init("Shaders/Holodecoder.vert", "Shaders/Holodecoder.frag");
	
		m_camera = new Camera();
		m_camera->init(0.0f, 1.0f, 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f);
	
		m_mesh->initMesh();
		haveHoloImage = false;
		
		m_hasBeenInit = true;
	}
}

void Holodecoder::draw(void)
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

void Holodecoder::resize(int width, int height)
{
	m_camera->reshape(width, height);
	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
}

void Holodecoder::mousePressEvent(int mouseX, int mouseY)
{
	m_camera->mousePressed(mouseX, mouseY);
}

void Holodecoder::mouseMoveEvent(int mouseX, int mouseY)
{
	m_camera->mouseMotion(mouseX, mouseY);
}

void Holodecoder::setHoloImage(GLuint holoImageTex)
{
	m_holoImageTex = holoImageTex;
}