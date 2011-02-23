/*
 Filename:		XYZFileIO.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/30/10
 Last Edited:	09/30/10
 
 Revision Log:
 09/30/10 - Nik Karpinsky - Original creation.
 */

#ifndef _XYZ_FILE_IO_H_
#define _XYZ_FILE_IO_H_

#include <iostream>
#include <fstream>

#include "AbstractMesh.h"
#include "XYZMesh.h"

using namespace std;

struct XYZHeader
{
	int version;
	int meshWidth;
	int meshHeight;
};

class XYZFileIO
{
public:
	XYZFileIO(void);
	~XYZFileIO(void);

	AbstractMesh* newMeshFromFile(const string &fileName);
};

#endif	// _XYZ_FILE_IO_H_