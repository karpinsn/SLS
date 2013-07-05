/*
 Filename:		AbstractMesh.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/01/09
 Last Edited:	09/01/09
 
 Revision Log:
 09/01/09 - Nik Karpinsky - Original creation.
 */

#ifndef _ABSTRACT_MESH_H_
#define _ABSTRACT_MESH_H_

#include <wrench/gl/utils/BoundingBox.h>
#include "XYZPoint.h"

using namespace std;
using namespace wrench::gl::utils;

class AbstractMesh
{
protected:
  BoundingBox m_boundingBox;

public:
	virtual ~AbstractMesh(void) {};
	virtual void initMesh(void) = 0;
	virtual void draw(void) = 0;

	virtual BoundingBox& getBoundingBox()
	{
	  return m_boundingBox;
	};
};

#endif	// _ABSTRACT_MESH_H_