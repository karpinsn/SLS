#include "PointCloudMesh.h"

PointCloudMesh::PointCloudMesh(int width, int height, int pixelsPerPoint)
{	
	m_meshWidth = width;
	m_meshHeight = height;
	m_pixelsPerPoint = pixelsPerPoint;
}

void PointCloudMesh::initMesh(void)
{
  	_generateTexturedVertices();
	_generateIndices();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	_cacheMesh();
}

void PointCloudMesh::draw()
{
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

  m_meshIndices.bind();
  m_mesh.draw();
}

void PointCloudMesh::_generateIndices(void)
{	
	unsigned int elementCount = (m_meshHeight / m_pixelsPerPoint) * (m_meshWidth / m_pixelsPerPoint);
	unique_ptr<GLuint[]> meshIndices = unique_ptr<GLuint[]>(new GLuint[elementCount]);
	
	for(unsigned int point = 0; point < elementCount; point++)
	{
		meshIndices[point] = point;
	}
	
	m_meshIndices.init(1, GL_UNSIGNED_INT);
	m_meshIndices.bufferData(elementCount, meshIndices.get(), GL_STATIC_DRAW);
}

void PointCloudMesh::_generateTexturedVertices(void)
{
	unsigned int elementCount = (m_meshHeight / m_pixelsPerPoint) * (m_meshWidth / m_pixelsPerPoint);
	unique_ptr<glm::vec3[]> meshVertices = unique_ptr<glm::vec3[]>(new glm::vec3[elementCount]);
	unique_ptr<glm::vec2[]> texCoord = unique_ptr<glm::vec2[]>(new glm::vec2[elementCount]);

	for(int row = 0; row < m_meshHeight; row += m_pixelsPerPoint)
	{
		for(int column = 0; column < m_meshWidth; column += m_pixelsPerPoint)
		{
			meshVertices[(row / m_pixelsPerPoint) * (m_meshHeight / m_pixelsPerPoint) + (column / m_pixelsPerPoint)].x = (float)column / (float)(m_meshWidth - 1.0);
			meshVertices[(row / m_pixelsPerPoint) * (m_meshHeight / m_pixelsPerPoint) + (column / m_pixelsPerPoint)].y = (float)row / (float)(m_meshHeight - 1.0);
			meshVertices[(row / m_pixelsPerPoint) * (m_meshHeight / m_pixelsPerPoint) + (column / m_pixelsPerPoint)].z = 0.0f;
			
			texCoord[(row / m_pixelsPerPoint) * (m_meshHeight / m_pixelsPerPoint) + (column / m_pixelsPerPoint)].x = (float)column / (float)m_meshWidth;
			texCoord[(row / m_pixelsPerPoint) * (m_meshHeight / m_pixelsPerPoint) + (column / m_pixelsPerPoint)].y = (float)row / (float)m_meshHeight;
		}
	}

	m_meshVertices.init(3, GL_FLOAT, GL_ARRAY_BUFFER);
	m_meshVertices.bufferData(m_meshHeight * m_meshWidth, glm::value_ptr(meshVertices[0]), GL_STATIC_DRAW);

	m_meshTextureCoords.init(2, GL_FLOAT, GL_ARRAY_BUFFER);
	m_meshTextureCoords.bufferData(m_meshHeight * m_meshWidth, glm::value_ptr(texCoord[0]), GL_STATIC_DRAW);
}

void PointCloudMesh::_cacheMesh(void)
{
  unsigned int elementCount = (m_meshHeight / m_pixelsPerPoint) * (m_meshWidth / m_pixelsPerPoint);
  m_mesh.init(GL_POINTS, elementCount);

  m_mesh.addVBO(m_meshVertices, "vert");
  m_mesh.addVBO(m_meshTextureCoords, "vertTexCoord");
}
