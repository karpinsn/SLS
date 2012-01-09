#include "XYZMCodec.h"

XYZMCodec::XYZMCodec(const QListWidget* xyzmList)
{
  m_listPosition = 0;
  m_xyzmList = xyzmList;
}

void XYZMCodec::openEncodeStream(EncodingOpenGLWidget* glWidget)
{
  //  Open the stream to write to
}

void XYZMCodec::previewEncode(MeshInterchange& data)
{
  return; // TODO comeback and fix this
}

void XYZMCodec::encode(MeshInterchange& data)
{
  //  Encode to the stream
}

void XYZMCodec::closeEncodeStream(void)
{
  //  Close the stream
}

void XYZMCodec::openDecodeStream(EncodingOpenGLWidget* glWidget)
{
  if(NULL == glWidget)
  {
	//  No OpenGL encoding widget. Cant open decode stream
	return;
  }

  m_glWidget = glWidget;
}

MeshInterchange* XYZMCodec::decode()
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a NULL MeshInterchange
    return NULL;
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
	return NULL;
  }

  return new MeshInterchange(mesh);
}

void XYZMCodec::closeDecodeStream(void)
{
  m_glWidget = NULL;
}

int XYZMCodec::getDecodeStreamWidth(void)
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

int XYZMCodec::getDecodeStreamHeight(void)
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

float XYZMCodec::getDecodeStreamProgress(void)
{
  float progress = 0.0f;
  
  if(NULL != m_xyzmList)
  {
	progress = (float)m_xyzmList->count()/(float)m_listPosition;
  }

  return progress;
}

string XYZMCodec::codecName(void)
{
  return "XYZM Codec";
}
