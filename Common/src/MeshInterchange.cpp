#include "MeshInterchange.h"

MeshInterchange::MeshInterchange()
{
  m_image = NULL;
  m_texture = NULL;
}

MeshInterchange::MeshInterchange(IplImage* image)
{
  m_texture = NULL;
  m_image = image;
}

MeshInterchange::MeshInterchange(Texture* texture)
{
  m_image = NULL;
  m_texture = texture;
}

int MeshInterchange::getHeight()
{
  return m_height;
}

int MeshInterchange::getWidth()
{
  return m_width;
}

bool MeshInterchange::isEmpty()
{
  //  If the texture and image are both NULL then its empty
  return NULL == m_texture && NULL == m_image;
}
