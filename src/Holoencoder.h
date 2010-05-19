#ifndef _Holoencoder_H
#define _Holoencoder_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/type_ptr.hpp>

#include <glew.h>
#include <QtOpenGL/QGLWidget>

#include "AOpenGLContext.h"
#include "ShaderFacade.h"
#include "Matrix.h"
#include "Camera.h"

using namespace std;

class Holoencoder : public AOpenGLContext 
{
private:
	ShaderFacade m_encoderShader;
	int m_width;
	int m_height;
	
	int m_previousX;
	int m_previousY;
	
	int m_translateX;
	int m_translateY;
	
	Camera* m_camera;
	
	glm::mat4 m_cameraProjectionMatrix;
	
public:
	virtual void init();
    virtual void draw(void);
	void encode(unsigned char* holoImage);
	virtual void resize(int width, int height);
	virtual void mousePressEvent(int mouseX, int mouseY);
	virtual void mouseMoveEvent(int mouseX, int mouseY);
	
private:
	void _drawBoundingCube(void);
};

#endif
