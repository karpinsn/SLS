#include "MeshInterchange.h"

MeshInterchange::MeshInterchange()
{
}

// Copy constructor.
MeshInterchange::MeshInterchange(const MeshInterchange& rhs)
{
  // Copy the data pointer and its length from the 
  // source object.
  m_image	  = rhs.m_image;
  m_texture	  = rhs.m_texture;
  m_mesh	  = rhs.m_mesh;
}

// Copy assignment operator.
MeshInterchange& MeshInterchange::operator=(const MeshInterchange& rhs)
{
  if (this != &rhs)
  {
	// Copy the data pointer and its length from the 
	// source object.
	m_image	  = rhs.m_image;
	m_texture  = rhs.m_texture;
	m_mesh	  = rhs.m_mesh;
  }
  return *this;
}

// Move constructor.
MeshInterchange::MeshInterchange(MeshInterchange&& rhs)
{
   // Copy the data pointer and its length from the 
   // source object.
   m_image	  = rhs.m_image;
   m_texture  = rhs.m_texture;
   m_mesh	  = rhs.m_mesh;

   // Release the data pointer from the source object so that
   // the memory is not freed.
   rhs.m_image		= nullptr;
   rhs.m_texture	= nullptr;
   rhs.m_mesh		= nullptr;
}

// Move assignment operator.
MeshInterchange& MeshInterchange::operator=(MeshInterchange&& rhs)
{
   if (this != &rhs)
   {
      // Copy the data pointer and its length from the 
      // source object.
      m_image = rhs.m_image;
      m_texture = rhs.m_texture;
	  m_mesh = rhs.m_mesh;

      // Release the data pointer from the source object so that
	  // the memory is not freed.
      rhs.m_image = nullptr;
      rhs.m_texture = nullptr;
	  rhs.m_mesh;
   }
   return *this;
}

MeshInterchange::MeshInterchange(IplImage* image, bool deleteAssets)
{
	if(deleteAssets)
	{
		m_image = shared_ptr<IplImage>(image, [](IplImage* ptr) { cvReleaseImage(&ptr); });
	}
	else
	{
		//	Blank destructor means we dont delete the assets
		m_image = shared_ptr<IplImage>(image, [](IplImage* ptr) {});
	}
}

MeshInterchange::MeshInterchange(Texture* texture, bool deleteAssets)
{
	if(deleteAssets)
	{
		m_texture = shared_ptr<Texture>(texture);
	}
	else
	{
		//	Blank destructor means we dont delete the assets
		m_texture = shared_ptr<Texture>(texture, [](Texture* ptr) {});
	}
}

MeshInterchange::MeshInterchange(AbstractMesh* mesh, bool deleteAssets)
{
	if(deleteAssets)
	{
		m_mesh = shared_ptr<AbstractMesh>(mesh);
	}
	else
	{
		//	Blank destructor means we dont delete the assets
		m_mesh = shared_ptr<AbstractMesh>(mesh, [](AbstractMesh* ptr) {});
	}
}

int MeshInterchange::getPreferedFormat(void)
{
  if(m_texture)
  {
	return TEXTURE_FORMAT;
  }
  else if(m_image)
  {
	return IMAGE_FORMAT;
  }
  else if(m_mesh)
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
  return m_texture.get();
}

IplImage* MeshInterchange::getIplImage(void)
{
  return m_image.get();
}

AbstractMesh* MeshInterchange::getMesh(void)
{
  return m_mesh.get();
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
  return !m_texture && !m_image && !m_mesh;
}
