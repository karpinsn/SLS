/*
 Filename:		XYZMesh.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	09/30/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 */

#ifndef _XYZ_MESH_H_
#define _XYZ_MESH_H_

#include <iostream>

#ifdef __APPLE__
	#include <glew.h>
	#include <OpenGL/gl.h>
#elif _WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QtOpenGL/QGLWidget>

#include <wrench/gl/utils/BoundingBox.h>

#include "AbstractMesh.h"
#include "XYZPoint.h"

using namespace std;

class XYZMesh : public AbstractMesh
{
private:
  int m_meshWidth;
  int m_meshHeight;
  shared_ptr<XYZPoint> m_meshPoints;
  
public:
	/**
	* Creates a new XYZ mesh with the specified width, height, and points
	*
	* @param meshWidth Width of the XYZ mesh
	* @param meshHeight Height of the XYZ mesh
	* @param meshPoints Ordered list of the XYZ points
	*/
	XYZMesh(int meshWidth, int meshHeight, shared_ptr<XYZPoint> meshPoints);

	/**
	* Initializes this mesh (Does nothing)
	*/
	virtual void initMesh(void);

	/**
	* Draws this mesh using direct drawing (depracated). This should only be
	* called from the OpenGL draw thread.
	*/
	virtual void draw(void);

	/**
	* Returns the width of this mesh
	*
	* @return Width of this mesh
	*/
	int getMeshWidth(void);

	/**
	* Returns the height of this mesh
	*
	* @return Height of this mesh
	*/
	int getMeshHeight(void);

	/**
	* Returns the total number of verticies in this mesh (MeshWidth * MeshHeight)
	*
	* @return Total number of verticies in this mesh
	*/
	int getMeshSize(void);

	/**
	* Sets the bounding box for this mesh
	*
	* @param BoundingBox corresponding the the bounding box of this mesh
	*/
	void setBoundingBox(BoundingBox& boundingBox);
};

#endif	// _XYZ_MESH_H_