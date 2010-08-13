#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtx/type_ptr.hpp>

#include <QtOpenGL/QGLWidget>

#include "AbstractMesh.h"
#include "XYZPoint.h"

using namespace std;

class XYZMesh : public AbstractMesh
{
public:
	XYZMesh(int meshWidth, int meshHeight, XYZPoint *meshPoints);
	~XYZMesh(void);

	virtual void draw(void);
	int getMeshWidth(void);
	int getMeshHeight(void);
	int getMeshSize(void);
	
private:
	int m_meshWidth;
	int m_meshHeight;
	XYZPoint *m_meshPoints;
	glm::vec4 m_normalVectors;
};