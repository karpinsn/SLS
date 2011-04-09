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

class TriMesh : public AbstractMesh
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
	#define m_triMeshVBOID   (*vrjVBOHandle)
	#define m_triMeshIBOID (*vrjIBOHandle)
	#else
		GLuint m_triMeshVBOID;
		GLuint m_triMeshIBOID;
	#endif
	
	unsigned int	*meshIndices;
	Vertex			*meshVertices;
	unsigned int	elementCount;
		
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
