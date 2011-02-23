/*
 Filename:		XYZViewer.h
 Author:		Nikolaus Karpinsky
 Date Created:	01/12/11
 Last Edited:	01/12/12
 
 Revision Log:
 01/12/12 - Nik Karpinsky - Original creation.
 */

#ifndef _XYZVIEWER_H_
#define _XYZVIEWER_H_

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

#include <wrench/gl/Shader.h>
#include <wrench/gl/Camera.h>
#include <wrench/gl/Arcball.h>
#include <wrench/gl/Texture.h>

#include "AbstractMesh.h"
#include "AbstractGLContext.h"

using namespace std;
using namespace wrench::gl;

class XYZViewer : public AbstractGLContext
{
private:
	Shader m_shader;
	int m_width;
	int m_height;
	
	Camera	m_camera;
	Arcball m_controller;
	
	AbstractMesh* m_currentMesh;
	
	bool m_hasBeenInit;
	
public:
	XYZViewer(void);
	
	virtual void				init();
    virtual void				draw(void);
	virtual void				resize(int width, int height);
	virtual void				cameraSelectMode(int mode);
	virtual void				mousePressEvent(int mouseX, int mouseY);
	virtual void				mouseMoveEvent(int mouseX, int mouseY);
	void						setCurrentMesh(AbstractMesh* current);
	
private:
	void						initShaders(void);
	void						_initLighting(void);
};

#endif	// _XYZVIEWER_H_
