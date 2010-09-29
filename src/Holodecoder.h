#ifndef _Holoviewer_H
#define _Holoviewer_H

#ifdef __APPLE__
	#include <glew.h>
	#include <QtOpenGL/QGLWidget>
#else
	#include <GL/glew.h>
	#include <windows.h>
	#include <QtOpenGL/QGLWidget>
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
	ShaderFacade m_decoderShader;
	
	ShaderFacade m_phaseCalculator;
	ShaderFacade m_phaseFilter;
	ShaderFacade m_normalCalculator;
	ShaderFacade m_finalRender;
	
	GLenum m_phaseMap0AttachPoint;
	GLenum m_phaseMap1AttachPoint;
	GLenum m_normalMapAttachPoint;
	
	TextureFacade m_phaseMap0;
	TextureFacade m_phaseMap1;
	TextureFacade m_normalMap;
	
	FBOFacade m_imageProcessor;
	
	Camera* m_camera;
	Arcball* m_controller;
	
	TriMesh* m_mesh;
	GLuint m_holoImageTex;
	
	bool haveHoloImage;
	
	bool m_hasBeenInit;
	
	//	Front and back buffers for drawing
	GLuint m_bufferIds[2];		//	PBO Buffers for the holoimage
	int m_frontBufferIndex;		//	Index of the front buffer in m_bufferIds
	
	OpenGLWidget* m_glContext;
	
public:
	Holodecoder(OpenGLWidget* glContext);
	
	virtual void init(void);
	virtual void draw(void);
	virtual void resize(int width, int height);
	virtual void mousePressEvent(int mouseX, int mouseY);
	virtual void mouseMoveEvent(int mouseX, int mouseY);
	void setHoloImage(GLuint holoImageTex);
	
	void setBackHoloBuffer(IplImage* image);
	void swapBuffers(void);
	
private:
	void initShaders(void);
	void _initTextures(GLuint width, GLuint height);
	void initHoloBuffers(void);
	void _initLighting(void);
	
	int DATA_SIZE;
	GLubyte* imageData;
};

#endif
