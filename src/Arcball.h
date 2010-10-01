/*
 Filename:		Arcball.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/01/09
 Last Edited:	09/01/09
 
 Revision Log:
 09/01/09 - Nik Karpinsky - Original creation.
 */

#ifndef _ARCBALL_H_
#define _ARCBALL_H_

#ifdef __APPLE__
	#include <QtOpenGL/QGLWidget>
#else _WIN32
	#include <windows.h>
	#include <QtOpenGL/QGLWidget>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/Quaternion.hpp>
#include <glm/gtx/type_ptr.hpp>

# define Epsilon 1.0e-5

class Arcball
{	
public:
	Arcball(GLfloat width, GLfloat height);
	virtual ~Arcball() {};
		
	//Mouse down
	void mousePressEvent(const GLint mouseX, const GLint mouseY);
	
	//Mouse drag, calculate rotation
	void mouseMoveEvent(const GLint mouseX, const GLint mouseY);
	
	glm::mat4 getTransform(void);
	void applyTransform(void);
	
protected:
	glm::vec3   m_startVector;          //Saved click vector
	glm::vec3   m_endVector;          //Saved drag vector
	GLfloat     m_adjustWidth;		//Mouse bounds width
	GLfloat     m_adjustHeight;		//Mouse bounds height
	
	glm::mat4 m_transform;
	glm::mat4 m_lastRotation;
	glm::mat4 m_thisRotation;
	
private:
	inline glm::vec3 mapPointToSphere(const glm::vec2& point) const;
	inline void setBounds(GLfloat width, GLfloat height);
};

#endif	// _ARCBALL_H_

