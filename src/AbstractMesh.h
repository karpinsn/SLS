#pragma once

#include "XYZPoint.h"

using namespace std;

class AbstractMesh
{
public:
	virtual ~AbstractMesh(void) {};

	virtual void draw(void) = 0;
};