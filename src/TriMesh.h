#ifndef TRI_MESH_HEADER
#define TRI_MESH_HEADER

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
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

#endif // TRI_MESH_HEADER