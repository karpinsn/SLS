#include "MeshInterchange.h"

MeshInterchange::MeshInterchange()
{
  m_deleteAssets = false;
  m_image = nullptr;
  m_texture = nullptr;
  m_mesh = nullptr;
}

MeshInterchange::MeshInterchange(IplImage* image, bool deleteAssets)
{
  m_deleteAssets = deleteAssets;
  m_texture = nullptr;
  m_image = image;
  m_mesh = nullptr;
}

MeshInterchange::MeshInterchange(Texture* texture, bool deleteAssets)
{
  m_deleteAssets = deleteAssets;
  m_texture = nullptr;
  m_image = nullptr;
  m_texture = texture;
  m_mesh = nullptr;
}

MeshInterchange::MeshInterchange(Texture& texture)
{
  m_deleteAssets = false;
  m_image = nullptr; 
  m_texture = nullptr;
  m_texture = &texture;
  m_mesh = nullptr;
}

MeshInterchange::MeshInterchange(AbstractMesh* mesh, bool deleteAssets)
{
  m_deleteAssets = deleteAssets;
  m_image = nullptr;
  m_texture = nullptr;
  m_mesh = mesh;
}

int MeshInterchange::getPreferedFormat(void)
{
  if(nullptr != m_texture)
  {
	return TEXTURE_FORMAT;
  }
  else if(nullptr != m_image)
  {
	return IMAGE_FORMAT;
  }
  else if(nullptr != m_data)
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
  if(m_deleteAssets)
  {
      _deleteAssets();
  }

  m_deleteAssets = deleteAssets;
  m_texture = texture;
}

void MeshInterchange::setIplImage(IplImage* image, bool deleteAssets)
{
  if(m_deleteAssets)
  {
      _deleteAssets();
  }

  m_deleteAssets = deleteAssets;
  m_image = image;
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
  //  If the texture and image are both nullptr then its empty
  return nullptr == m_texture && nullptr == m_image && nullptr == m_mesh;
}

void MeshInterchange::_deleteAssets(void)
{
  if(nullptr != m_texture)
  {
      delete m_texture;
  }

  if(nullptr != m_mesh)
  {
      delete m_mesh;
  }

  if(nullptr != m_image)
  {
      cvReleaseImage(&m_image);
  }

  m_image   = nullptr;
  m_texture = nullptr;
  m_mesh    = nullptr; 
}
