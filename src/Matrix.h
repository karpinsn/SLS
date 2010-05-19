#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

/*
	Filename:		Matrix.h
	Author:			Nikolaus Karpinsky
	Date Created:	10/29/09
	Last Edited:	10/29/09
 
	Revision Log:
	10/29/09 - Nik Karpinsky - Initial creation.
*/

#include <stdio.h>

/*
	Structure that represents a 4x4 matrix.  The matrix is column
	major, like matricies in OpenGL
*/
struct Matrix
{
	//	Array that holds the matrix values.
	float m[16];
	
	/*
		Default constructor that creates a new Matrix.  It is initalized 
		to be the identity matrix. 
	 */
	Matrix()
	{
		for(int i=0; i<16; i++) 
		{
			m[i] = 0.0f;
		}
		m[0] = 1.0f;  m[5] = 1.0f;  m[10] = 1.0f;  m[15] = 1.0f;
	}
	
	/*
		Creates a matrix from the specified float array.
	 */
	Matrix(const float matrix[16])
	{
		for(int i=0; i<16; i++) 
		{
			m[i] = matrix[i];
		}
	}
	
	/*
		Copy constructor which creates a matrix from the specified matrix
	 */
	Matrix(const Matrix& matrix)
    {
        for(int i=0; i<16; i++) 
        {
            m[i] = matrix.m[i];
        }
    }

	/*
		Creates a new matrix using the specified entries
	 */
    Matrix(float a0,  float a1,  float a2,  float a3, 
           float a4,  float a5,  float a6,  float a7, 
           float a8,  float a9,  float a10, float a11,
           float a12 ,float a13, float a14, float a15)
    {
      m[0] = a0;  m[4] = a4; m[8]  = a8;  m[12] = a12;
      m[1] = a1;  m[5] = a5; m[9]  = a9;  m[13] = a13; 
      m[2] = a2;  m[6] = a6; m[10] = a10; m[14] = a14;
      m[3] = a3;  m[7] = a7; m[11] = a11; m[15] = a15;  
    }

	/*
		Sets this matrix to the identity matrix
	 */
    void Identity()
    {
        for(int i=0; i<16; i++)
		{
			m[i] = 0.0f;
		}
        m[0] = 1.0f;  m[5] = 1.0f;  m[10] = 1.0f;  m[15] = 1.0f;
    }
	
	/*
		Prints out the values of the matrix to the console
	 */
    void Print()
    {
        printf("Matrix = \n");
        printf("[%5.2f %5.2f %5.2f %5.2f]\n", m[0], m[4], m[8],  m[12]);
        printf("[%5.2f %5.2f %5.2f %5.2f]\n", m[1], m[5], m[9],  m[13]); 
        printf("[%5.2f %5.2f %5.2f %5.2f]\n", m[2], m[6], m[10], m[14]); 
        printf("[%5.2f %5.2f %5.2f %5.2f]\n", m[3], m[7], m[11], m[15]); 
    }

	/*
		Operator overload for =.  Sets this matricies values equal to the 
		specified matrix
	 */
    Matrix& operator= (const Matrix& matrix)
    {
        for(int i=0;i<16;i++)
	    {
		    this->m[i] = matrix.m[i];
	    }
        return *this;
    }
};

#endif