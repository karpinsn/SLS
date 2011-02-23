#include "TriMesh.h"

TriMesh::TriMesh(int width, int height)
{	
}

TriMesh::~TriMesh()
{
	glDeleteBuffers(1, &m_triMeshVBOID);
	glDeleteBuffers(1, &m_triMeshIBOID);
}

void TriMesh::initMesh(void)
{
	_generateTexturedVertices();
	_generateIndices();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	_cacheTriMesh();
}

void TriMesh::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_triMeshVBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triMeshIBOID);
	glDrawElements(GL_TRIANGLE_STRIP, elementCount, GL_UNSIGNED_INT, NULL);
}

void TriMesh::_generateIndices(void)
{
	const int meshWidth = 512;
	const int meshHeight = 512;
	
	elementCount = (meshWidth * 2) * (meshHeight - 1) + (meshHeight - 2);
	meshIndices = new unsigned int[elementCount];
	
	int index = 0;
	for(int row = 0; row < meshHeight - 1; row++)
	{
		//	Check if we are on an even row
		if(row % 2 == 0)
		{
			int x;
			for(x = 0; x < meshWidth; x++)
			{
				meshIndices[index++] = x + (row * meshWidth);
				meshIndices[index++] = x + (row * meshWidth) + meshWidth;
			}
			if(row != meshHeight -2)
			{
				//	Insert degenerate vertex to preserve the front face
				meshIndices[index++] = --x + (row * meshWidth);
			}
		}
		//	Odd Row
		else
		{
			int x;
			for(x = meshWidth - 1; x >= 0; x--)
			{
				meshIndices[index++] = x + (row * meshWidth);
				meshIndices[index++] = x + (row * meshWidth) + meshWidth;
			}
			
			if(row != meshHeight - 2)
			{
				meshIndices[index++] = ++x + (row * meshWidth);
			}
		}
	}
}

void TriMesh::_generateTexturedVertices(void)
{
	int const meshWidth = 512;
	int const meshHeight = 512;
	
	meshVertices = new Vertex[meshHeight * meshWidth];
	for(int row = 0; row < meshHeight; row++)
	{
		for(int column = 0; column < meshWidth; column++)
		{
			meshVertices[row * meshHeight + column].x = (float)row * 0.001953125 -.5 + .001;
			meshVertices[row * meshHeight + column].y = (float)column * 0.001953125 + .5;
			meshVertices[row * meshHeight + column].z = 0.0f;
			
			meshVertices[row * meshHeight + column].u = (float)row / (float)meshHeight;
			meshVertices[row * meshHeight + column].v = (float)column / (float)meshWidth;
		}
	}
}

void TriMesh::_cacheTriMesh(void)
{
	glGenBuffers(1, &m_triMeshVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, m_triMeshVBOID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * elementCount, meshVertices, GL_STATIC_DRAW);
	
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	
	glGenBuffers(1, &m_triMeshIBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triMeshIBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elementCount, meshIndices, GL_STATIC_DRAW);
}
