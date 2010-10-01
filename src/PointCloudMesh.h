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
	#include <QtOpenGL/QGLWidget>
#else _WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <QtOpenGL/QGLWidget>
#endif

#include <stdio.h>
#include <string.h>

struct Vertex 
{
	float x;
	float y;
	float z;
	float u;
	float v;
};

class PointCloudMesh
{
private:
	void _generateIndices(void);
	void _generateTexturedVertices(void);
	void _cacheMesh(void);
	
	Vertex *meshVertices;
	unsigned int *meshIndices;
	unsigned int elementCount;
	int m_pixelsPerPoint;
	
	unsigned int m_meshVBOID;
	unsigned int m_meshIBOID;
	
	int m_meshWidth;
	int m_meshHeight;
public:
	PointCloudMesh(int width, int height, int pixelsPerPoint);
	~PointCloudMesh();
	virtual void initMesh(void);
	
	void Draw();
};

#endif // _POINT_CLOUD_MESH_H_