#include "XYZMDecoder.h"

XYZMDecoder::XYZMDecoder(const QListWidget* xyzmList)
{
  m_listPosition = 0;
  m_xyzmList = xyzmList;
}

void XYZMDecoder::openCodec(EncodingOpenGLWidget* glWidget)
{
  if(NULL == glWidget)
  {
	//  No OpenGL encoding widget. Cant open decode stream
	return;
  }

  m_glWidget = glWidget;
}

void XYZMDecoder::closeCodec(void)
{
  m_glWidget = NULL;
}

void XYZMDecoder::process(MeshInterchange* data)
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a NULL MeshInterchange
    data = NULL;
    return;
  }

  AbstractMesh* mesh = NULL;
  if(NULL != m_xyzmList && m_listPosition < m_xyzmList->count())
  {
	QListWidgetItem *item = m_xyzmList->item(m_listPosition);
	mesh = m_io.newMeshFromFile(item->text().toAscii().constData());
	m_listPosition++; // Dont forget to increment the position
  }

  if(NULL == mesh)
  {
	//	At the end of the file
    data = NULL;
    return;
  }

  //  TODO Comeback and fix this
  data->setMesh(mesh);
}

void XYZMDecoder::previewProcess(MeshInterchange* data)
{

}

int XYZMDecoder::getWidth(void)
{
  if(NULL != m_xyzmList && m_listPosition < m_xyzmList->count())
  {
	QListWidgetItem *item = m_xyzmList->item(m_listPosition);
	AbstractMesh* mesh = m_io.newMeshFromFile(item->text().toAscii().constData());
	int width = ((XYZMesh*)mesh)->getMeshWidth();
	delete mesh;

	return width;
  }
  
  return 0;
}

int XYZMDecoder::getHeight(void)
{
  if(NULL != m_xyzmList && m_listPosition < m_xyzmList->count())
  {
	QListWidgetItem *item = m_xyzmList->item(m_listPosition);
	AbstractMesh* mesh = m_io.newMeshFromFile(item->text().toAscii().constData());
	int height = ((XYZMesh*)mesh)->getMeshHeight();
	delete mesh;

	return height;
  }
  
  return 0;
}

float XYZMDecoder::getStreamLocation(void)
{
  float progress = 0.0f;
  
  if(NULL != m_xyzmList)
  {
	progress = (float)m_xyzmList->count()/(float)m_listPosition;
  }

  return progress;
}

string XYZMDecoder::codecName(void)
{
  return "XYZM Decoder";
}
