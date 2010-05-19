#ifndef POINT_CLOUD_MESH_HEADER
#define POINT_CLOUD_MESH_HEADER

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/gl.h>
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

#endif // POINT_CLOUD_MESH_HEADER