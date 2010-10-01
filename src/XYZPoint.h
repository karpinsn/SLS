/*
	Filename:		XYZPoint.h
	Author:			Nikolaus Karpinsky
	Date Created:	09/01/09
	Last Edited:	09/01/09

	Revision Log:
		09/01/09 - Nik Karpinsky - Original creation.
*/

#ifndef _XYZ_POINT_H_
#define _XYZ_POINT_H_

#include <glm/glm.hpp>

/*
	Structure that represents a texture vertex in 3 space.
*/
struct TextureVertex
{
	unsigned char v[3];
};

/*
	Structure that represents a XYZPoint.
*/
struct XYZPoint
{
	//	Holds the vertex of the point in 3 space
	glm::vec3 vertex;
	
	//	Texture coordinate to map the point to a texture
	TextureVertex textureVertex;

	//	Value that determines whether or not this point should
	//	be drawn
	unsigned char valid;
	
	//	Normal vector of the point.
	glm::vec4 normalVector;
};

#endif	// _XYZ_POINT_H_