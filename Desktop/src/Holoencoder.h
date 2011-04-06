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
	#include <OpenGL/gl.h>
#elif _WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <wrench/gl/Shader.h>
#include <wrench/gl/ShaderProgram.h>
#include <wrench/gl/Camera.h>
#include <wrench/gl/Arcball.h>
#include <wrench/gl/Texture.h>

#include "AbstractMesh.h"
#include "AbstractGLContext.h"

using namespace std;
using namespace wrench::gl;

class Holoencoder : public AbstractGLContext
{
private:
        ShaderProgram m_encoderShader;
	int m_width;
	int m_height;
	
	Camera* m_camera;
	Arcball m_controller;
        glm::mat4 m_cameraModelView;
	
	AbstractMesh* m_currentMesh;
	
	Texture m_holoimage;
	
	//	Render to texture items
	GLuint m_holoimageFBO;
	GLuint m_holoimageRBO;
	
	bool m_hasBeenInit;
	
	glm::mat4 m_transform;
	glm::mat4 m_lastRotation;
	glm::mat4 m_thisRotation;
	
public:
	Holoencoder(void);
	
        virtual void                        init();
        virtual void                        draw(void);
        wrench::gl::Texture&                encode();
        virtual void                        resize(int width, int height);
        virtual void                        cameraSelectMode(int mode);
        virtual void                        mousePressEvent(int mouseX, int mouseY);
        virtual void                        mouseMoveEvent(int mouseX, int mouseY);
        void                                setCurrentMesh(AbstractMesh* current);
	
private:
        void _initFBO(void);
        void _initShaders(void);
	
};

#endif	// _HOLOENCODER_H_
