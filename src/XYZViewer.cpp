#include "XYZViewer.h"

XYZViewer::XYZViewer(void)
{
	m_hasBeenInit = false;
	m_currentMesh = NULL;
}

void XYZViewer::init()
{
	if(!m_hasBeenInit)
	{
		initShaders();
		_initLighting();
		
		m_controller.init(512, 512);
		m_camera.init(0.0f, 0.75f, 1.0f, 0.0f, 0.75f, 0.0f, 0.0f, 1.0f, 0.0f);
		m_camera.setMode(1);
		
		m_currentMesh = NULL;
		
		m_hasBeenInit = true;
	}
}

void XYZViewer::initShaders(void)
{
	//	Create the shaders
	m_shader.init("Shaders/Phong.vert", "Shaders/Phong.frag");
	
	OGLStatus::logOGLErrors("Holodecoder - initShaders()");
}

void XYZViewer::draw(void)
{	
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	
	m_camera.applyMatrix();
	m_controller.applyTransform();
	
	
	glm::mat4 translateMatrix = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
										  0.0f, 1.0f, 0.0f, 0.0f,
										  0.0f, 0.0f, 1.0f, 0.0f,
										  0.0f, -.1f, 0.7f, 1.0f);
	
	glm::mat4 scaleMatrix	  = glm::mat4(2.0f, 0.0f, 0.0f, 0.0f,
										  0.0f, 2.0f, 0.0f, 0.0f,
										  0.0f, 0.0f, 2.0f, 0.0f,
										  0.0f, 0.0f, 0.0f, 1.0f);
	
	glm::mat4 rotationMatrix  = glm::mat4(0.99026806874157f, 0.0f, 0.139173100960065f, 0.0f,
										  0.0f			   , 1.0f, 0.0f,               0.0f,
										  -0.139173100960065f, 0.0f, 0.99026806874157f,0.0f,
										  0.0f, 0.0f, 0.0f, 1.0f);
	
	glMultMatrixf(glm::value_ptr(translateMatrix));
	glMultMatrixf(glm::value_ptr(scaleMatrix));
	glMultMatrixf(glm::value_ptr(rotationMatrix));
	//glRotatef(8.0f, .0, 1.0f, 0.0f);
	
	glColor3f(1.165f, 0.775f, 0.645f);
		
	if(NULL != m_currentMesh)
	{	m_shader.bind();
		{
			m_currentMesh->draw();
		}
		m_shader.unbind();
	}
	
	glPopMatrix();
	
	OGLStatus::logOGLErrors("Holodecoder - draw()");
}

void XYZViewer::resize(int width, int height)
{
	m_camera.reshape(width, height);
	gluPerspective(45.0, 1.0, .00001, 10.0);
	//glFrustum(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
	//glOrtho(-10.0, 10.0, -10.0, 10.0, -10.0, 10.0);
}

void XYZViewer::cameraSelectMode(int mode)
{
	m_camera.setMode(mode);
}

void XYZViewer::mousePressEvent(int mouseX, int mouseY)
{
	m_camera.mousePressed(mouseX, mouseY);
}

void XYZViewer::mouseMoveEvent(int mouseX, int mouseY)
{
	m_camera.mouseMotion(mouseX, mouseY);
}

void XYZViewer::_initLighting(void)
{
	GLfloat materialSpecular[] = {.5f, .5f, .45f, 1.0f};
	GLfloat materialShininess[] = {1.0f};
	
	GLfloat lightPosition[] = {0.0f, 0.0f, 1.0f, 0.0f};
	GLfloat lightAmbient[] = {0.05f, 0.05f, 0.05f, 1.0f};
	GLfloat lightDiffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
	GLfloat lightSpecular[] = {0.6f, 0.6f, 0.6f, 1.0f};
	
	glClearColor(1.0, 1.0, 1.0, 0.0);
	
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, materialShininess);
	
	//	Setup light 0
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	
	//	Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void XYZViewer::setCurrentMesh(AbstractMesh* current)
{
	if(NULL != m_currentMesh)
	{
		//	Make sure to delete the current mesh if its not null
		delete m_currentMesh;
	}
	
	m_currentMesh = current;
}