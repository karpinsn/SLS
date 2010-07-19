#pragma once

#include <iostream>
#include <fstream>

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

	XYZMesh* newMeshFromFile(const string &fileName);
};