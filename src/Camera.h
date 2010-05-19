#pragma once

#include <QtOpenGL/QGLWidget>

#include <math.h>

#include "Matrix.h"
#include "Vector.h"

using namespace std;

#define DegToRad 3.14159f/180.0f
#define RadToDeg 180.0f/3.14159f

/* 
 *  Simple trackball-like motion adapted from 
 *  "Mathematical Elements for Computer Graphics" by David F. Rogers &
 *  Alan J. Adams 1976. Section 3-8, pp. 55 ~ 59.
 *  Implemented by Tsung-Pin Yeh 10/26/2005. Modified 10/11/2006
*/

class Camera
{
	
public:
    Camera();
    ~Camera(void);

/* functions */
    void setMode(int mode);
    void init();
	void init(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
	void initRotatedCam(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
	
    void applyMatrix();
    void reshape(int width, int height);
    void mousePressed(int x, int y);
	void mouseRelease(int x, int y);
    void mouseMotion(int x, int y);
	
protected:
    GLint     tb_mode;  // 1: rotate
                        // 2: Zoom
                        // 3: Twist
                        // 4: Pan

    GLboolean tb_moving;
    GLfloat   tb_currPos[2];
    GLfloat   tb_prevPos[2];
    
	Matrix*   tb_transform;
    Matrix*   tb_accuTransform;

	float	  m_viewLength;
    GLuint    tb_width;
    GLuint    tb_height;
	int		  m_currentView;
		
	void _getScreenXYZ(float sx[3], float sy[3], float sz[3]);
    void _rotate(float dx, float dy);
    void _zoom(float dx, float dy);
    void _twist(float dx, float dy);
    void _pan(float dx, float dy);
    void _atRotate(float x,  float y,  float z,  float angle);
};