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
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include "AbstractMesh.h"
#include <stdio.h>
#include <string.h>

#ifdef USE_VRJ
#include <vrj/Draw/OpenGL/ContextData.h>
#endif

class PointCloudMesh : public AbstractMesh
{
private:
	struct Vertex 
	{
		float x;
		float y;
		float z;
		float u;
		float v;
	};
	
	#ifdef USE_VRJ
        vrj::opengl::ContextData<GLuint> vrjVBOHandle;
        vrj::opengl::ContextData<GLuint> vrjIBOHandle;
		#define m_meshVBOID		(*vrjVBOHandle)
		#define m_meshIBOID		(*vrjIBOHandle)
	#else
		GLuint m_meshVBOID;
		GLuint m_meshIBOID;
	#endif

	Vertex *meshVertices;
	unsigned int *meshIndices;
	unsigned int elementCount;
	int m_pixelsPerPoint;
	
	int m_meshWidth;
	int m_meshHeight;
	
public:
	PointCloudMesh(int width, int height, int pixelsPerPoint);
	~PointCloudMesh();
	virtual void initMesh(void);
	
	void draw();
	
private:
	void _generateIndices(void);
	void _generateTexturedVertices(void);
	void _cacheMesh(void);
};

#endif // _POINT_CLOUD_MESH_H_
