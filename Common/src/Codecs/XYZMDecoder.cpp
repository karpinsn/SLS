#include "XYZMDecoder.h"

XYZMDecoder::XYZMDecoder(const QListWidget* xyzmList)
{
  m_listPosition = 0;
  m_xyzmList = xyzmList;
}

void XYZMDecoder::openCodec(EncodingOpenGLWidget* glWidget)
{
  if(nullptr == glWidget)
  {
	//  No OpenGL encoding widget. Cant open decode stream
	return;
  }

  m_glWidget = glWidget;
}

void XYZMDecoder::closeCodec(void)
{
  m_glWidget = nullptr;
}

shared_ptr<MeshInterchange> XYZMDecoder::process(shared_ptr<MeshInterchange> data)
{
  if(nullptr == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a nullptr MeshInterchange
    data = nullptr;
    return shared_ptr<MeshInterchange>();
  }

  AbstractMesh* mesh = nullptr;
  if(nullptr != m_xyzmList && m_listPosition < m_xyzmList->count())
  {
	QListWidgetItem *item = m_xyzmList->item(m_listPosition);
	mesh = m_io.newMeshFromFile(item->text().toAscii().constData());
	m_listPosition++; // Dont forget to increment the position
  }

  return shared_ptr<MeshInterchange>(new MeshInterchange(mesh));
}

shared_ptr<MeshInterchange> XYZMDecoder::previewProcess(shared_ptr<MeshInterchange> data)
{
  if(nullptr == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a nullptr MeshInterchange
    return nullptr;
  }

  AbstractMesh* mesh = nullptr;
  if(nullptr != m_xyzmList && m_listPosition < m_xyzmList->count())
  {
	QListWidgetItem *item = m_xyzmList->item(m_listPosition);
	mesh = m_io.newMeshFromFile(item->text().toAscii().constData());
	//	We dont increment since we are just previewing
  }

  return shared_ptr<MeshInterchange>(new MeshInterchange(mesh));
}

int XYZMDecoder::getWidth(void)
{
  if(nullptr != m_xyzmList && m_listPosition < m_xyzmList->count())
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
  if(nullptr != m_xyzmList && m_listPosition < m_xyzmList->count())
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
  
  if(nullptr != m_xyzmList)
  {
	progress = (float)m_xyzmList->count()/(float)m_listPosition;
  }

  return progress;
}

string XYZMDecoder::codecName(void)
{
  return "XYZM Decoder";
}
