/**
  * @file   MeshInterchange.h
  * @author Nikolaus Karpinsky
  * @data   8/11/2011
  *
  * Format used to exchange common mesh data between different mesh codecs
  */

#ifndef _MESH_INTERCHANGE_H_
#define _MESH_INTERCHANGE_H_

#include <memory>

//  Includes for the data types that we support.
#include <cv.h>
#include <highgui.h>
#include <wrench/gl/Texture.h>
#include "AbstractMesh.h"

using namespace std;
using namespace wrench::gl;

class MeshInterchange
{
public:
  static const int TEXTURE_FORMAT = 0;
  static const int IMAGE_FORMAT = 1;
  static const int VERTEX_FORMAT = 2;

private:
  int         m_width;
  int         m_height;

  shared_ptr<Texture>		m_texture;
  shared_ptr<IplImage>		m_image;
  shared_ptr<AbstractMesh>	m_mesh;

  bool m_deleteAssets;

public:
  MeshInterchange();
  MeshInterchange(IplImage* image, bool deleteAssets = true);
  MeshInterchange(Texture* texture, bool deleteAssets = true);
  MeshInterchange(AbstractMesh* mesh, bool deleteAssets = true);

  int getPreferedFormat(void);

  Texture*	getTexture(void);
  IplImage* getIplImage(void);
  AbstractMesh* getMesh(void);

  int getWidth();
  int getHeight();
  bool isEmpty();
};

#endif // _MESH_INTERCHANGE_H_
