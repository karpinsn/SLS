/**
  * @file   MeshInterchange.h
  * @author Nikolaus Karpinsky
  * @data   8/11/2011
  *
  * Format used to exchange common mesh data between different mesh codecs
  */

#ifndef _MESH_INTERCHANGE_H_
#define _MESH_INTERCHANGE_H_

//  GLM Includes
#include <glm/glm.hpp>

//  OpenCV Includes
#include <cv.h>
#include <highgui.h>

//  Wrench Includes
#include <wrench/gl/Texture.h>

#include "AbstractMesh.h"

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

  Texture*    m_texture;
  IplImage*   m_image;
  glm::vec3*  m_data;
  AbstractMesh* m_mesh;

public:
  MeshInterchange();
  MeshInterchange(IplImage* image);
  MeshInterchange(Texture* texture);
  MeshInterchange(Texture& texture);
  MeshInterchange(AbstractMesh* mesh);

  int getPreferedFormat(void);

  Texture*	getTexture(void);
  IplImage* getIplImage(void);
  AbstractMesh* getMesh(void);

  int getWidth();
  int getHeight();
  bool isEmpty();
};

#endif // _MESH_INTERCHANGE_H_
