#include "PointCloudMesh.h"

PointCloudMesh::PointCloudMesh(int width, int height, int pixelsPerPoint)
{	
	m_meshWidth = width;
	m_meshHeight = height;
	m_pixelsPerPoint = pixelsPerPoint;
	_generateTexturedVertices();
	_generateIndices();
}

PointCloudMesh::~PointCloudMesh()
{
	glDeleteBuffers(1, &m_meshVBOID);
	glDeleteBuffers(1, &m_meshIBOID);
}

void PointCloudMesh::initMesh(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	_cacheMesh();
}

void PointCloudMesh::Draw()
{
	/*
	glPolygonMode(GL_FRONT, GL_POINT);

	glBindBuffer(GL_ARRAY_BUFFER, m_meshVBOID);			//	Bind the vertex coordinates
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshIBOID);	//	Bind the indices

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawElements(GL_TRIANGLE_STRIP, elementCount, GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshVBOID);			//	Unbind the vertex coordinates
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshIBOID);	//	Unbind the indices
	 */
	
	glPolygonMode(GL_FRONT, GL_POINT);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshVBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshIBOID);
	glDrawElements(GL_TRIANGLE_STRIP, elementCount, GL_UNSIGNED_INT, NULL);
}

void PointCloudMesh::_generateIndices(void)
{	
	elementCount = (m_meshHeight / m_pixelsPerPoint) * (m_meshWidth / m_pixelsPerPoint);
	meshIndices = new unsigned int[elementCount];
	
	for(unsigned int point = 0; point < elementCount; point++)
	{
		meshIndices[point] = point;
	}
	
}

void PointCloudMesh::_generateTexturedVertices(void)
{
	meshVertices = new Vertex[(m_meshHeight / m_pixelsPerPoint) * (m_meshWidth / m_pixelsPerPoint)];
	for(int row = 0; row < m_meshHeight; row += m_pixelsPerPoint)
	{
		for(int column = 0; column < m_meshWidth; column += m_pixelsPerPoint)
		{
			meshVertices[(row / m_pixelsPerPoint) * (m_meshHeight / m_pixelsPerPoint) + (column / m_pixelsPerPoint)].x = (float)row * 0.001953125 -.5 + .001;
			meshVertices[(row / m_pixelsPerPoint) * (m_meshHeight / m_pixelsPerPoint) + (column / m_pixelsPerPoint)].y = (float)column * 0.001953125 + .5;
			meshVertices[(row / m_pixelsPerPoint) * (m_meshHeight / m_pixelsPerPoint) + (column / m_pixelsPerPoint)].z = 0.0f;
			
			meshVertices[(row / m_pixelsPerPoint) * (m_meshHeight / m_pixelsPerPoint) + (column / m_pixelsPerPoint)].v = (float)column / (float)m_meshWidth;
			meshVertices[(row / m_pixelsPerPoint) * (m_meshHeight / m_pixelsPerPoint) + (column / m_pixelsPerPoint)].u = (float)row / (float)m_meshHeight;
		}
	}
}



void PointCloudMesh::_cacheMesh(void)
{
	glGenBuffers(1, &m_meshVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, m_meshVBOID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * elementCount, meshVertices, GL_STATIC_DRAW);
	
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	
	glGenBuffers(1, &m_meshIBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_meshIBOID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elementCount, meshIndices, GL_STATIC_DRAW);
}