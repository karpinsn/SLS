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
#include "Camera.h"
#include "ImageIO.h"
#include "AbstractGLContext.h"

#include "ShaderFacade.h"
#include "TextureFacade.h"
#include "FBOFacade.h"

#include "OpenGLWidget.h"
#include "OGLStatus.h"

#include "Arcball.h"

class OpenGLWidget;

class Holodecoder : public AbstractGLContext
{
private:
	ShaderFacade m_phaseCalculator;
	ShaderFacade m_phaseFilter;
	ShaderFacade m_normalCalculator;
	ShaderFacade m_finalRender;
	
	GLenum m_phaseMap0AttachPoint;
	GLenum m_phaseMap1AttachPoint;
	GLenum m_normalMapAttachPoint;
	
	TextureFacade* m_holoImages[2];
	TextureFacade m_holoImage0;
	TextureFacade m_holoImage1;
	
	TextureFacade m_phaseMap0;
	TextureFacade m_phaseMap1;
	TextureFacade m_normalMap;
	
	FBOFacade m_imageProcessor;
	
	Camera* m_camera;
	Arcball* m_controller;
	
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
