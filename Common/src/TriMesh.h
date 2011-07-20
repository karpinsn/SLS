/**
 @file  		TriMesh.h
 @author:		Nikolaus Karpinsky


*/

#ifndef _TRI_MESH_H_
#define _TRI_MESH_H_

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

#include <wrench/gl/IBO.h>
#include <wrench/gl/VBO.h>
#include <wrench/gl/VAO.h>

using namespace wrench::gl;

class TriMesh : public AbstractMesh
{
private:
	struct Vertex 
	{
      float x;
      float y;
      float z;
	};

    struct TextureCoordinate
    {
      float u;
      float v;
    };
	
    VAO m_mesh;
    IBO m_meshIndices;
    VBO m_meshVertices;
    VBO m_meshTextureCoords;

    unsigned int	m_elementCount;
    unsigned int    m_width;
    unsigned int    m_height;
		
public:
	TriMesh(int width, int height);
	~TriMesh();
	virtual void initMesh(void);
	
	void draw();
	
private:
	void _generateIndices(void);
	void _generateTexturedVertices(void);
	void _cacheTriMesh(void);
};

#endif // _TRI_MESH_H_
