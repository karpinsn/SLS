/*
 Filename:		TriMesh.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/01/09
 Last Edited:	09/01/09
 
 Revision Log:
 09/01/09 - Nik Karpinsky - Original creation.
 */

#ifndef _TRI_MESH_H_
#define _TRI_MESH_H_

#ifdef __APPLE__
	#include <glew.h>
	#include <QtOpenGL/QGLWidget>
#else _WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <QtOpenGL/QGLWidget>
#endif

#include <stdio.h>
#include <string.h>

class TriMesh
{
	struct Vertex 
	{
		float x;
		float y;
		float z;
		float u;
		float v;
	};
	
private:
	void _generateIndices(void);
	void _generateTexturedVertices(void);
	void _cacheTriMesh(void);
	
	unsigned int *meshIndices;
	Vertex *meshVertices;
	unsigned int elementCount;
	
	unsigned int m_triMeshVBOID;
	unsigned int m_triMeshIBOID;
public:
	TriMesh(int width, int height);
	~TriMesh();
	virtual void initMesh(void);
	
	void Draw();
};

#endif // _TRI_MESH_H_