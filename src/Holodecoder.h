#ifndef _Holoviewer_H
#define _Holoviewer_H

#include <glew.h>
#include <QtOpenGL/QGLWidget>

#include "PointCloudMesh.h"
#include "Camera.h"
#include "ShaderFacade.h"
#include "ImageIO.h"
#include "AbstractGLContext.h"

class Holodecoder : public AbstractGLContext
{
private:
	ShaderFacade m_decoderShader;
	Camera* m_camera;
	
	PointCloudMesh* m_mesh;
	GLuint m_holoImageTex;
	
	bool haveHoloImage;
	
	bool m_hasBeenInit;
	
public:
	Holodecoder(void);
	
	virtual void init(void);
	virtual void draw(void);
	virtual void resize(int width, int height);
	virtual void mousePressEvent(int mouseX, int mouseY);
	virtual void mouseMoveEvent(int mouseX, int mouseY);
	void setHoloImage(GLuint holoImageTex);
	
};

#endif
