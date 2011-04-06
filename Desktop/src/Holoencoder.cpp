#include "Holoencoder.h"

Holoencoder::Holoencoder(void)
{
	m_hasBeenInit = false;
        m_currentMesh = NULL;
}

void Holoencoder::init()
{	
	if(!m_hasBeenInit)
	{
		m_width = 512;
		m_height = 512;
	
		m_currentMesh = NULL;
                _initFBO();
	
		m_controller.init(m_width, m_height);
                _initShaders();

		m_camera = new Camera();
                m_camera->init(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		m_camera->setMode(4);
		
		//	Define the camera projection matrix
                m_cameraModelView = glm::mat4();
                m_cameraModelView = m_cameraModelView * glm::gtx::transform::rotate(30.0f, 0.0f, 1.0f, 0.0f);


		m_hasBeenInit = true;
	}
}

void Holoencoder::_initFBO()
{
        //m_holoimage.init(m_width, m_height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
        m_holoimage.init(m_width, m_height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT);

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
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_holoimageFBO);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	
	m_camera->applyMatrix();
	
	//	Draw the currentMesh
        glm::mat4 cameraModelViewMatrix = m_cameraModelView;
	
	
	


        glm::mat4 scaleMatrix	  = glm::mat4(2.0f, 0.0f, 0.0f, 0.0f,
                                                                                  0.0f, 2.0f, 0.0f, 0.0f,
                                                                                  0.0f, 0.0f, 2.0f, 0.0f,
										  0.0f, 0.0f, 0.0f, 1.0f);

        glm::mat4 translateMatrix = glm::mat4(  1.0f, 0.0f, 0.0f, -0.5f,
                                                               0.0f, 1.0f, 0.0f,  -0.5f,
                                                               0.0f, 0.0f, 1.0f,  .0f,
                                                               0.0f, 0.0f, 0.0f,  1.0f);
                       translateMatrix = glm::transpose(translateMatrix);

	
	
	//glTranslatef(0.0f, 0.0f, 0.8f);
        //
        glMultMatrixf(glm::value_ptr(scaleMatrix));
        glMultMatrixf(glm::value_ptr(translateMatrix));
	//glTranslatef(0.0f, -0.1f, 0.7f);
	//glScalef(2.4f, 2.4f, 2.4f);
	
        cameraModelViewMatrix = cameraModelViewMatrix * m_controller.getTransform() * scaleMatrix * translateMatrix;
	
        m_encoderShader.bind();
        m_encoderShader.uniformMat4("projectorModelView", false, glm::value_ptr(cameraModelViewMatrix));
	
        GLUquadricObj *quadratic = gluNewQuadric();

        //  Experiment 1
        //gluSphere(quadratic, 1.0, 4096, 4096);


        //  Experiment 2
//        glBegin(GL_QUADS);
//        glVertex4f(-.75, -.75, 0.0, 1);    // Lower left
//        glVertex4f(.75, -.75, 0.0, 1); //  Lower right
//        glVertex4f(.75, .75, 0.0, 1);  //  Upper right
//        glVertex4f(-.75, .75, 0.0, 1); //  Upper left

//        glEnd();

        //  Experiment 2.5

//        glm::mat4 translateMatrix = glm::mat4(  1.0f, 0.0f, 0.0f, -0.5f,
//                                                       0.0f, 1.0f, 0.0f,  -0.5f,
//                                                       0.0f, 0.0f, 1.0f,  0.0f,
//                                                       0.0f, 0.0f, 0.0f,  1.0f);
//               translateMatrix = glm::transpose(translateMatrix);
//               glm::mat4 mat = cameraModelViewMatrix;
//               mat = mat * translateMatrix;
//               m_encoderShader.uniformMat4("projectorModelView", false, glm::value_ptr(mat));
//               glMultMatrixf(glm::value_ptr(translateMatrix));

        //  Experiment 3
        //{
        //  Upper left

//        glPushMatrix();
//        glm::mat4 translateMatrix = glm::mat4(  1.0f, 0.0f, 0.0f, -0.5f,
//                                                0.0f, 1.0f, 0.0f,  0.5f,
//                                                0.0f, 0.0f, 1.0f,  0.0f,
//                                                0.0f, 0.0f, 0.0f,  1.0f);
//        translateMatrix = glm::transpose(translateMatrix);
//        glm::mat4 mat = cameraModelViewMatrix;
//        mat = mat * translateMatrix;
//        m_encoderShader.uniformMat4("projectorModelView", false, glm::value_ptr(mat));
//        glMultMatrixf(glm::value_ptr(translateMatrix));
//        gluSphere(quadratic, .4, 4096, 4096);
//        glPopMatrix();

//        //  lower left
//        glPushMatrix();
//                  translateMatrix = glm::mat4(  1.0f, 0.0f, 0.0f, -0.5f,
//                                                0.0f, 1.0f, 0.0f, -0.5f,
//                                                0.0f, 0.0f, 1.0f,  0.0f,
//                                                0.0f, 0.0f, 0.0f,  1.0f);
//        translateMatrix = glm::transpose(translateMatrix);
//        mat = cameraModelViewMatrix;
//        mat = mat * translateMatrix;
//        m_encoderShader.uniformMat4("projectorModelView", false, glm::value_ptr(mat));
//        glMultMatrixf(glm::value_ptr(translateMatrix));
//        gluSphere(quadratic, .4, 4096, 4096);
//        glPopMatrix();

//        //  Upper right
//        glPushMatrix();
//                  translateMatrix = glm::mat4(  1.0f, 0.0f, 0.0f,  0.5f,
//                                                0.0f, 1.0f, 0.0f,  0.5f,
//                                                0.0f, 0.0f, 1.0f,  0.0f,
//                                                0.0f, 0.0f, 0.0f,  1.0f);
//        translateMatrix = glm::transpose(translateMatrix);
//        mat = cameraModelViewMatrix;
//        mat = mat * translateMatrix;
//        m_encoderShader.uniformMat4("projectorModelView", false, glm::value_ptr(mat));
//        glMultMatrixf(glm::value_ptr(translateMatrix));
//        gluSphere(quadratic, .4, 4096, 4096);
//        glPopMatrix();

//        //  lower right
//        glPushMatrix();
//                  translateMatrix = glm::mat4(  1.0f, 0.0f, 0.0f,  0.5f,
//                                                0.0f, 1.0f, 0.0f, -0.5f,
//                                                0.0f, 0.0f, 1.0f,  0.0f,
//                                                0.0f, 0.0f, 0.0f,  1.0f);
//        translateMatrix = glm::transpose(translateMatrix);
//        mat = cameraModelViewMatrix;
//        mat = mat * translateMatrix;
//        m_encoderShader.uniformMat4("projectorModelView", false, glm::value_ptr(mat));
//        glMultMatrixf(glm::value_ptr(translateMatrix));
//        gluSphere(quadratic, .4, 4096, 4096);
//        glPopMatrix();

        //}

        glColor3f(.8, .8, .8);
        m_controller.applyTransform();
	if(NULL != m_currentMesh)
	{
		m_currentMesh->draw();
	}
	
        m_encoderShader.unbind();
	
	glPopMatrix();

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

Texture& Holoencoder::encode()
{	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_holoimageFBO);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
	
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	
	return m_holoimage;
}

void Holoencoder::resize(int width, int height)
{
	m_width = width;
	m_height = height;
        glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	//glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 100.0);
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
