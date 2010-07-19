#include "Holoencoder.h"

void Holoencoder::init()
{
	m_encoderShader.init("Shaders/Holoencoder.vert", "Shaders/Holoencoder.frag");
	
	m_translateX = 0;
	m_translateY = 0;
	m_width = 512;
	m_height = 512;

	m_camera = new Camera();
	m_camera->initRotatedCam(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	
	//	Define the camera projection matrix
	m_cameraProjectionMatrix = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
										 0.0f, 1.0f, 0.0f, 0.0f,
										 0.0f, 0.0f, 1.0f, 0.0f,
										 0.0f, 0.0f, -1.0f, 1.0f);
}

void Holoencoder::draw(void)
{	
	m_camera->applyMatrix();
	//	Draw the meshes
	//_drawBoundingCube();
	
	glm::mat4 cameraModelViewMatrix = m_cameraProjectionMatrix;
	glPushMatrix();

	//	Need to perform the operation on both the model view and cameraModelView
	glTranslatef((float)m_translateX/(float)m_width, (float)m_translateY/(float)m_height, 0.0f);
	cameraModelViewMatrix = glm::translate(cameraModelViewMatrix, glm::vec3((float)m_translateX/(float)m_width, (float)m_translateY/(float)m_height, 0.0f));

	m_encoderShader.bind();
	GLint projectorModelViewLoc = glGetUniformLocation(m_encoderShader.shaderID(), "projectorModelView");
	glUniformMatrix4fv(projectorModelViewLoc, 16, false, glm::value_ptr(cameraModelViewMatrix));//m_cameraModelView.m);
	
	GLUquadric* quad = gluNewQuadric();
	gluSphere(quad, .5f, 128, 128);
	m_encoderShader.unbind();
	
	glPopMatrix();
	cameraModelViewMatrix = m_cameraProjectionMatrix;
	
	m_encoderShader.bind();
	
	projectorModelViewLoc = glGetUniformLocation(m_encoderShader.shaderID(), "projectorModelView");
	glUniformMatrix4fv(projectorModelViewLoc, 16, false, glm::value_ptr(cameraModelViewMatrix));

	GLUquadric* quad2 = gluNewQuadric();
	gluSphere(quad2, .5f, 128, 128);
	m_encoderShader.unbind();
}

void Holoencoder::encode(unsigned char* holoImage)
{
	glReadBuffer( GL_FRONT );
	glReadPixels(0, 0, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, holoImage);
}

void Holoencoder::resize(int width, int height)
{
	m_width = width;
	m_height = height;
	glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 100.0);
}

void Holoencoder::mousePressEvent(int mouseX, int mouseY)
{
	m_previousX = mouseX;
	m_previousY = mouseY;
}

void Holoencoder::mouseMoveEvent(int mouseX, int mouseY)
{
	m_translateX = mouseX - m_previousX;
	m_translateY = -(mouseY - m_previousY);
}