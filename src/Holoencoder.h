/*
 Filename:		Holoencoder.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	09/30/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 */

#ifndef _HOLOENCODER_H_
#define _HOLOENCODER_H_

#ifdef __APPLE__
	#include <glew.h>
	#include <QtOpenGL/QGLWidget>
#else _WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <QtOpenGL/QGLWidget>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/type_ptr.hpp>

#include "ShaderFacade.h"
#include "Camera.h"
#include "AbstractMesh.h"
#include "AbstractGLContext.h"
#include "Arcball.h"

using namespace std;

class Holoencoder : public AbstractGLContext
{
private:
	ShaderFacade m_encoderShader;
	int m_width;
	int m_height;
	
	Camera* m_camera;
	Arcball* m_controller;
	glm::mat4 m_cameraProjectionMatrix;
	
	AbstractMesh* m_currentMesh;
	
	//	Render to texture items
	GLuint m_holoimageFBO;
	GLuint m_holoimageRBO;
	GLuint m_holoimageTextureID;
	
	bool m_hasBeenInit;
	
	glm::mat4 m_transform;
	glm::mat4 m_lastRotation;
	glm::mat4 m_thisRotation;
	
public:
	Holoencoder(void);
	
	virtual void init();
    virtual void draw(void);
	GLuint encode();
	virtual void resize(int width, int height);
	virtual void mousePressEvent(int mouseX, int mouseY);
	virtual void mouseMoveEvent(int mouseX, int mouseY);
	void setCurrentMesh(AbstractMesh* current);
	
private:
	void initFBO(void);
	
};

#endif	// _HOLOENCODER_H_
