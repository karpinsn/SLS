#include "TriMesh.h"

TriMesh::TriMesh(int width, int height) : m_width(width), m_height(height)
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

void TriMesh::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_triMeshVBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triMeshIBOID);
    glDrawElements(GL_TRIANGLE_STRIP, m_elementCount, GL_UNSIGNED_INT, NULL);
}

void TriMesh::_generateIndices(void)
{
    //  More elements than the width * height due to genenerate verticies
    m_elementCount = (m_width * 2) * (m_height - 1) + (m_height - 2);
    m_meshIndices = new unsigned int[m_elementCount];
	
	int index = 0;
    for(int row = 0; row < m_height - 1; row++)
	{
		//	Check if we are on an even row
		if(row % 2 == 0)
		{
			int x;
            for(x = 0; x < m_width; x++)
			{
                m_meshIndices[index++] = x + (row * m_width);
                m_meshIndices[index++] = x + (row * m_width) + m_width;
			}
            if(row != m_height -2)
			{
				//	Insert degenerate vertex to preserve the front face
                m_meshIndices[index++] = --x + (row * m_width);
			}
		}
		//	Odd Row
		else
		{
			int x;
            for(x = m_width - 1; x >= 0; x--)
			{
                m_meshIndices[index++] = x + (row * m_width);
                m_meshIndices[index++] = x + (row * m_width) + m_width;
			}
			
            if(row != m_height - 2)
			{
                m_meshIndices[index++] = ++x + (row * m_width);
			}
		}
	}
}

void TriMesh::_generateTexturedVertices(void)
{
    m_meshVertices = new Vertex[m_height * m_width];
    for(int row = 0; row < m_height; row++)
	{
        for(int column = 0; column < m_width; column++)
		{
            m_meshVertices[row * m_height + column].x = (float)row / m_height;
            m_meshVertices[row * m_height + column].y = (float)column / m_width;
            m_meshVertices[row * m_height + column].z = 0.0f;
			
            m_meshVertices[row * m_height + column].u = (float)row / (float)m_height;
            m_meshVertices[row * m_height + column].v = (float)column / (float)m_width;
		}
	}
}

void TriMesh::_cacheTriMesh(void)
{
	glGenBuffers(1, &m_triMeshVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, m_triMeshVBOID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_width * m_height, m_meshVertices, GL_STATIC_DRAW);
	
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	
	glGenBuffers(1, &m_triMeshIBOID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_triMeshIBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_elementCount, m_meshIndices, GL_STATIC_DRAW);
}
