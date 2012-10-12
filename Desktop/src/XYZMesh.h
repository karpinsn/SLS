/*
 Filename:		XYZMesh.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	09/30/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 */

#ifndef _XYZ_MESH_H_
#define _XYZ_MESH_H_

#include <iostream>

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
#include <glm/gtc/type_ptr.hpp>

#include <QtOpenGL/QGLWidget>

#include <wrench/gl/utils/BoundingBox.h>

#include "AbstractMesh.h"
#include "XYZPoint.h"

using namespace std;

class XYZMesh : public AbstractMesh
{
public:
	XYZMesh(int meshWidth, int meshHeight, XYZPoint *meshPoints);
	~XYZMesh(void);

	virtual void initMesh(void);
	virtual void draw(void);
	int getMeshWidth(void);
	int getMeshHeight(void);
	int getMeshSize(void);

	void setBoundingBox(BoundingBox& boundingBox);
	
private:
	int m_meshWidth;
	int m_meshHeight;
	XYZPoint *m_meshPoints;
	glm::vec4 m_normalVectors;
};

#endif	// _XYZ_MESH_H_