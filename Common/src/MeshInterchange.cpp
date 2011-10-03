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

MeshInterchange::MeshInterchange(Texture& texture)
{
  m_texture = &texture;
}

MeshInterchange::MeshInterchange(AbstractMesh* mesh)
{
  m_mesh = mesh;
}

int MeshInterchange::getPreferedFormat(void)
{
  if(NULL != m_texture)
  {
	return TEXTURE_FORMAT;
  }
  else if(NULL != m_image)
  {
	return IMAGE_FORMAT;
  }
  else if(NULL != m_data)
  {
	return VERTEX_FORMAT;
  }
  else
  {
	return -1;
  }
}

Texture* MeshInterchange::getTexture(void)
{
  return m_texture;
}

IplImage* MeshInterchange::getIplImage(void)
{
  return m_image;
}

AbstractMesh* MeshInterchange::getMesh(void)
{
  return m_mesh;
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