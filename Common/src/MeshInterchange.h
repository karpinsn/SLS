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

using namespace wrench::gl;

class MeshInterchange
{
private:
  int         m_width;
  int         m_height;

  glm::vec3*  m_data;

public:
  MeshInterchange();
  MeshInterchange(IplImage* image);
  MeshInterchange(Texture* texture);

  //  TODO: Come back and fix this
  Texture*    m_texture;
  IplImage*   m_image;

  int getWidth();
  int getHeight();
  bool isEmpty();
};

#endif // _MESH_INTERCHANGE_H_
