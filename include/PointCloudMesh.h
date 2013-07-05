/*
 Filename:		PointCloudMesh.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/01/09
 Last Edited:	09/01/09
 
 Revision Log:
 09/01/09 - Nik Karpinsky - Original creation.
 */

#ifndef _POINT_CLOUD_MESH_H_
#define _POINT_CLOUD_MESH_H_

#ifdef __APPLE__
	#include <glew.h>
	#include <OpenGL/gl.h>
#elif _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include <stdio.h>
#include <string.h>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <wrench/gl/IBO.h>
#include <wrench/gl/VBO.h>
#include <wrench/gl/VAO.h>

#include "AbstractMesh.h"

using namespace wrench::gl;

class PointCloudMesh : public AbstractMesh
{
private:
    VAO m_mesh;
    IBO m_meshIndices;
    VBO m_meshVertices;
    VBO m_meshTextureCoords;

	int m_pixelsPerPoint;
	int m_meshWidth;
	int m_meshHeight;
	
public:
	PointCloudMesh(int width, int height, int pixelsPerPoint);
	virtual void initMesh(void);
	
	void draw();
	
private:
	void _generateIndices(void);
	void _generateTexturedVertices(void);
	void _cacheMesh(void);
};

#endif // _POINT_CLOUD_MESH_H_
