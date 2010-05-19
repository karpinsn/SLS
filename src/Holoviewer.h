#ifndef _Holoviewer_H
#define _Holoviewer_H

#include <glew.h>
#include <QtOpenGL/QGLWidget>

#include "AOpenGLContext.h"

#include "PointCloudMesh.h"
#include "Camera.h"
#include "ShaderFacade.h"

class Holoviewer : public AOpenGLContext 
{
private:
	ShaderFacade m_decoderShader;
	Camera* m_camera;
	
	PointCloudMesh* m_mesh;
	GLuint m_holoImageTex;
	
	bool haveHoloImage;
	
public:
	virtual void init();
	virtual void draw(void);
	virtual void resize(int width, int height);
	virtual void mousePressEvent(int mouseX, int mouseY);
	virtual void mouseMoveEvent(int mouseX, int mouseY);
	void setHoloImage(unsigned char*);
	
};

#endif
