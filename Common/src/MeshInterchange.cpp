#include "MeshInterchange.h"

MeshInterchange::MeshInterchange()
{
  m_deleteAssets = false;
  m_image = NULL;
  m_texture = NULL;
  m_mesh = NULL;
}

MeshInterchange::MeshInterchange(IplImage* image, bool deleteAssets)
{
  m_deleteAssets = deleteAssets;
  m_texture = NULL;
  m_image = image;
  m_mesh = NULL;
}

MeshInterchange::MeshInterchange(Texture* texture, bool deleteAssets)
{
  m_deleteAssets = deleteAssets;
  m_texture = NULL;
  m_image = NULL;
  m_texture = texture;
  m_mesh = NULL;
}

MeshInterchange::MeshInterchange(Texture& texture)
{
  m_deleteAssets = false;
  m_image = NULL; 
  m_texture = NULL;
  m_texture = &texture;
  m_mesh = NULL;
}

MeshInterchange::MeshInterchange(AbstractMesh* mesh, bool deleteAssets)
{
  m_deleteAssets = deleteAssets;
  m_image = NULL;
  m_texture = NULL;
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

void MeshInterchange::setTexture(Texture* texture, bool deleteAssets)
{
}

void MeshInterchange::setIplImage(IplImage* image, bool deleteAssets)
{
}

void MeshInterchange::setMesh(AbstractMesh* mesh, bool deleteAssets)
{
  if(m_deleteAssets)
  {
      _deleteAssets();
  }

  m_deleteAssets = deleteAssets; 
  m_mesh = mesh;
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
  return NULL == m_texture && NULL == m_image && NULL == m_mesh;
}

void Meshinterchange::_deleteAssets(void)
{
  if(NULL != m_texture)
  {
      delete m_texture;
  }

  if(NULL != m_mesh)
  {
      delete m_mesh;
  }

  if(NULL != m_image)
  {
      cvReleaseImage(&m_image);
  }

  m_image   = NULL;
  m_texture = NULL;
  m_mesh    = NULL; 
}
