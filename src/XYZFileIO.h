#pragma once

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