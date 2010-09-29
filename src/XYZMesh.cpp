#include "XYZMesh.h"

XYZMesh::XYZMesh(int meshWidth, int meshHeight, XYZPoint *meshPoints)
{
	m_meshHeight = meshHeight;
	m_meshWidth = meshWidth;
	m_meshPoints = meshPoints;
}

XYZMesh::~XYZMesh(void)
{
	//	Delete all the points that we have
	delete [] m_meshPoints;
}

void XYZMesh::draw(void)
{
	int intvl = 1;//sampleInterval;

	//calculatePointNormals();

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < m_meshHeight - intvl; i += intvl)
	{
		int lineNo = i * m_meshWidth;				
		for (int j = 0; j < m_meshWidth - intvl; j += intvl)
		{
			int id0 = lineNo + j;
			int id1 = id0 + m_meshWidth * intvl;
			int id2 = id1 + intvl;
			int id3 = id0 + intvl;
			XYZPoint f0 = m_meshPoints[id0];
			XYZPoint f1 = m_meshPoints[id1];
			XYZPoint f2 = m_meshPoints[id2];
			XYZPoint f3 = m_meshPoints[id3];

			if (f0.valid && f1.valid && f2.valid)
			{
				glNormal3fv(glm::value_ptr(f0.normalVector));
				glVertex3fv(glm::value_ptr(f0.vertex));
				glNormal3fv(glm::value_ptr(f1.normalVector));
				glVertex3fv(glm::value_ptr(f1.vertex));
				glNormal3fv(glm::value_ptr(f2.normalVector));
				glVertex3fv(glm::value_ptr(f2.vertex));
			}

			if (f0.valid && f2.valid && f3.valid)
			{
				glNormal3fv(glm::value_ptr(f0.normalVector));
				glVertex3fv(glm::value_ptr(f0.vertex));
				glNormal3fv(glm::value_ptr(f2.normalVector));
				glVertex3fv(glm::value_ptr(f2.vertex));
				glNormal3fv(glm::value_ptr(f3.normalVector));
				glVertex3fv(glm::value_ptr(f3.vertex));
			}
		}				
	}
	glEnd();
}

int XYZMesh::getMeshHeight(void)
{
	return m_meshHeight;
}

int XYZMesh::getMeshWidth(void)
{
	return m_meshWidth;
}

int XYZMesh::getMeshSize(void)
{
	return m_meshWidth * m_meshHeight;
}