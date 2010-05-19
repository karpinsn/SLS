#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/*
	Filename:		Vector.h
	Author:			Nikolaus Karpinsky
	Date Created:	08/26/09
	Last Edited:	10/29/09

	Revision Log:
		10/29/09 - Nik Karpinsky - Added print method and copy constructor.
		08/26/09 - Nik Karpinsky - Original creation.  Added 3
			constructors, crossproduct method, and normalize method.
*/

#include <math.h>
#include <stdio.h>

/*
	Structure that represents a vector in 3 space.  Contains
	methods to perform simple operations such as computing the
	cross product and normalizing the vector
*/
struct Vector
{
	//	Array that holds the coordiates for the vector
	float v[3];

	/*
		Default constructor that creates a new Vector that
		goes down the x axis.  This vector is normalized.
	*/
	Vector()
	{
		v[0] = 1;
		v[1] = 0;
		v[2] = 0;
	}

	/*
		Creates a vector from the specified float array
	 */
	Vector(const float *vector)
	{
		v[0] = vector[0];
		v[1] = vector[1];
		v[2] = vector[2];
	}
	
	/*
		Copy constructor which creates a vector from the specified vector
	 */
	Vector(const Vector& vector)
    {
        v[0] = vector.v[0];
		v[1] = vector.v[1];
		v[2] = vector.v[2];
    }
	
	/*
		Creates a new vector using the specified points.
		Assumes 0,0,0 as the origin vertex.  Does not normalize
		the vector.

		Params:
			float x - X coordinate for the vector
			float y - Y coordinate for the vector
			float z - Z coordinate for the vector
	*/
	Vector(float x, float y, float z)
	{
		v[0] = x;
		v[1] = y;
		v[2] = z;
	}

	/*
		Creates a new vector from the specified originVertex
		and directionalVertex.  Does not normalize the vector.

		Params:
			float originVectex[] - Vertex to be used as the origin.
				This array must be 3 floats in length (x, y, z).
			float directionalVertex[] - Vertex used to determine the
				direction of the vector.  This array must be 3
				floats in length (x, y, z).
	*/
	Vector(float originVertex[3], float directionalVertex[3])
	{
		v[0] = directionalVertex[0] - originVertex[0];
		v[1] = directionalVertex[1] - originVertex[1];
		v[2] = directionalVertex[2] - originVertex[2];
	}

	/*
	 Creates a new vector from the specified originVertex
	 and directionalVertex.  Does not normalize the vector.
	 
	 Params:
		float originalX - X component of vertex to be used as the origin.
		float originalY - Y component of vertex to be used as the origin.
		float originalZ - Z component of vertex to be used as the origin.
		float directionalX - X component of vertex to be used as direction
		float directionalY - Y component of vertex to be used as direction
		float directionalZ - Z component of vertex to be used as direction
	 */
	Vector(float originalX, float originalY, float originalZ, float directionalX, float directionalY, float directionalZ)
	{
		v[0] = directionalX - originalX;
		v[1] = directionalY - originalY;
		v[2] = directionalZ - originalZ;
	}
	
	/*
		Crosses this vector with the vectorToCross.  Resulting
		vector is stored in returnVector

		Params:
			Vector *vectorToCross	- Vector to calculate cross product with
			Vector *returnVector	- Vector resulting from cross product
	*/
	void crossProduct(Vector *vectorToCross, Vector *returnVector)
	{
		returnVector->v[0] = (v[1] * vectorToCross->v[2] - vectorToCross->v[1] * v[2]);
		returnVector->v[1] = (v[2] * vectorToCross->v[0] - vectorToCross->v[2] * v[0]);
		returnVector->v[2] = (v[0] * vectorToCross->v[1] - vectorToCross->v[0] * v[1]);
	}
	
	/*
		Produces the dot product between this vector and vectorToDot.
	 
		Params:
			Vector &vectorToDot		- Vector to perform dot product with
	 */
	float dotProduct(const Vector &vectorToDot)
	{
		return v[0] * vectorToDot.v[0] + v[1] * vectorToDot.v[1] + v[2] * vectorToDot.v[2];
	}

	/*
		Simple normalizes the vector to a unit vector
		with a magnitude of 1
	*/
	void normalize()
	{
		//	Find the length of the vector
		float lenght = sqrt((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2])); 

		//	Divide all the components by the length
		v[0] /= lenght;
		v[1] /= lenght;
		v[2] /= lenght;
	}
	
	/*
		Prints out the values of the vector to the console
	 */
	void print()
    {
        printf("Vector = ");
        printf("[%5.2f %5.2f %5.2f]\n", v[0], v[1], v[2]);
    }
	
	/*
		Operator overload for =.  Sets this vector values equal to the 
		specified vector
	 */
    Vector& operator= (const Vector& vector)
    {
		this->v[0] = vector.v[0];
		this->v[1] = vector.v[1];
		this->v[2] = vector.v[2];
		
        return *this;
    }
};

#endif