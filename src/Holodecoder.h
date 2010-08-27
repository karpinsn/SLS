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

#include "PointCloudMesh.h"
#include "Camera.h"
#include "ShaderFacade.h"
#include "ImageIO.h"
#include "AbstractGLContext.h"

#include "OpenGLWidget.h"

class OpenGLWidget;

class Holodecoder : public AbstractGLContext
{
private:
	ShaderFacade m_decoderShader;
	Camera* m_camera;
	
	PointCloudMesh* m_mesh;
	GLuint m_holoImageTex;
	
	bool haveHoloImage;
	
	bool m_hasBeenInit;
	
	bool frontBufferFull;
	
	//	Front and back buffers for drawing
	GLuint m_bufferIds[2];		//	PBO Buffers for the holoimage
	int m_frontBufferIndex;		//	Index of the front buffer in m_bufferIds
	
	OpenGLWidget* m_glContext;
	
	QMutex m_glMutex;
	
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
	void lockAndMakeCurrent(void);
	void unlock(void);
	void printGLErrors(void);
	int DATA_SIZE;
	GLubyte* imageData;
};

#endif
