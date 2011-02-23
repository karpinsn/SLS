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

#include "XYZPoint.h"

using namespace std;

class AbstractMesh
{
public:
	virtual ~AbstractMesh(void) {};

	virtual void draw(void) = 0;
};

#endif	// _ABSTRACT_MESH_H_