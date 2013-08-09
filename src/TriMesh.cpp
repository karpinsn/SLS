#include "TriMesh.h"

TriMesh::TriMesh(int width, int height) : m_width(width), m_height(height)
{ }

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
  glDisable(GL_CULL_FACE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

  m_meshIndices.bind();
  m_mesh.draw();
}

void TriMesh::_generateIndices(void)
{
  //  More elements than the width * height due to genenerate verticies
  m_elementCount = (m_width * 2) * (m_height - 1) + (m_height - 2);
  unique_ptr<GLuint[]> indicies = unique_ptr<GLuint[]>(new GLuint[m_elementCount]);

  int index = 0;
  for(unsigned int row = 0; row < m_height - 1; row++)
  {
    //	Check if we are on an even row
    if(row % 2 == 0)
    {
      unsigned int x;
      for(x = 0; x < m_width; x++)
      {
        indicies[index++] = x + (row * m_width) + m_width;
        indicies[index++] = x + (row * m_width);
      }
      if(row != m_height -2)
      {
        //	Insert degenerate vertex to preserve the front face
        indicies[index++] = --x + (row * m_width);
      }
    }
    //	Odd Row
    else
    {
      int x;
      for(x = m_width - 1; x >= 0; x--)
      {
        indicies[index++] = x + (row * m_width) + m_width;
        indicies[index++] = x + (row * m_width);
      }

      if(row != m_height - 2)
      {
        indicies[index++] = ++x + (row * m_width);
      }
    }
  }

  m_meshIndices.init(1, GL_UNSIGNED_INT);
  m_meshIndices.bufferData(m_elementCount, indicies.get(), GL_STATIC_DRAW);
}

void TriMesh::_generateTexturedVertices(void)
{
  unique_ptr<glm::vec3[]> verticies = unique_ptr<glm::vec3[]>(new glm::vec3[m_height * m_width]);
  unique_ptr<glm::vec2[]> texCoord = unique_ptr<glm::vec2[]>(new glm::vec2[m_height * m_width]);

  for(unsigned int row = 0; row < m_height; row++)
  {
    for(unsigned int column = 0; column < m_width; column++)
    {
      //  - 1.0 on the width and height is so that the values range from (0, 1)
      verticies[row * m_width + column].x = (float)column / (float)(m_width - 1.0);
      verticies[row * m_width + column].y = (float)row / (float)(m_height - 1.0);
      verticies[row * m_width + column].z = 0.0f;

      texCoord[row * m_width + column].s = (float)column / (float)(m_width - 1.0);
      texCoord[row * m_width + column].t = (float)row / (float)(m_height - 1.0);
    }
  }

  m_meshVertices.init(3, GL_FLOAT, GL_ARRAY_BUFFER);
  m_meshVertices.bufferData(m_height * m_width, glm::value_ptr(verticies[0]), GL_STATIC_DRAW);

  m_meshTextureCoords.init(2, GL_FLOAT, GL_ARRAY_BUFFER);
  m_meshTextureCoords.bufferData(m_height * m_width, glm::value_ptr(texCoord[0]), GL_STATIC_DRAW);

}

void TriMesh::_cacheTriMesh(void)
{
  m_mesh.init(GL_TRIANGLE_STRIP, m_elementCount);
  m_mesh.addIBO(m_meshIndices);

  m_mesh.addVBO(m_meshVertices, "vert");
  m_mesh.addVBO(m_meshTextureCoords, "vertTexCoord");
}
