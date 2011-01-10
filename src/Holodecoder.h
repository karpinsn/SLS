/*
 Filename:		Holodecoder.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/01/09
 Last Edited:	09/01/09
 
 Revision Log:
 09/01/09 - Nik Karpinsky - Original creation.
 */

#ifndef _HOLODECODER_H_
#define _HOLODECODER_H_

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

#include <QMutex>
#include <cv.h>

#include "TriMesh.h"
#include "PointCloudMesh.h"
#include "ImageIO.h"
#include "AbstractGLContext.h"

#include <wrench/gl/Camera.h>
#include <wrench/gl/Shader.h>
#include <wrench/gl/Texture.h>
#include <wrench/gl/FBO.h>

#include "OpenGLWidget.h"
#include <wrench/gl/OGLStatus.h>

#include <wrench/gl/Arcball.h>

class OpenGLWidget;

using namespace wrench;
using namespace wrench::gl;

class Holodecoder : public AbstractGLContext
{
public:
	Shader m_phaseCalculator;
	Shader m_phaseFilter;
	Shader m_normalCalculator;
	Shader m_finalRender;
	
	GLenum m_phaseMap0AttachPoint;
	GLenum m_phaseMap1AttachPoint;
	GLenum m_normalMapAttachPoint;
	
	Texture* m_holoImages[2];
	Texture m_holoImage0;
	Texture m_holoImage1;
	
	Texture m_phaseMap0;
	Texture m_phaseMap1;
	Texture m_normalMap;
	
	FBO m_imageProcessor;
	
	Camera m_camera;
	Arcball m_controller;
	
	TriMesh* m_mesh;
	
	bool haveHoloImage;
	
	bool m_hasBeenInit;
	
	int m_frontBufferIndex;		//	Index of the front buffer in m_bufferIds
	
	OpenGLWidget* m_glContext;
	
public:
	Holodecoder(OpenGLWidget* glContext);
	
	virtual void init(void);
	virtual void draw(void);
	virtual void resize(int width, int height);
	virtual void cameraSelectMode(int mode);
	virtual void mousePressEvent(int mouseX, int mouseY);
	virtual void mouseMoveEvent(int mouseX, int mouseY);
	
	void setBackHoloBuffer(IplImage* image);
	void swapBuffers(void);
	
private:
	void initShaders(void);
	void _initTextures(GLuint width, GLuint height);
	void _initLighting(void);
	
	int DATA_SIZE;
	GLubyte* imageData;
};

#endif	// _HOLODECODER_H_
