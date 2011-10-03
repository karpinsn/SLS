#include "XYZMCodec.h"

void XYZMCodec::openEncodeStream(EncodingOpenGLWidget* glWidget, string& filename, int width, int height)
{
  //  Open the stream to write to

}

void XYZMCodec::encode(MeshInterchange& data)
{
  //  Encode to the stream
}

void XYZMCodec::closeEncodeStream(void)
{
  //  Close the stream
}

void XYZMCodec::openDecodeStream(EncodingOpenGLWidget* glWidget, string& filename)
{
  if(NULL == glWidget)
  {
	//  No OpenGL encoding widget. Cant open decode stream
	return;
  }

  m_mesh = m_io.newMeshFromFile(filename);
  m_glWidget = glWidget;
}

MeshInterchange* XYZMCodec::decode()
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a NULL MeshInterchange
    return NULL;
  }

  if(NULL == m_mesh)
  {
	//	At the end of the file
	return NULL;
  }

  return new MeshInterchange(m_mesh);
}

void XYZMCodec::closeDecodeStream(void)
{
  m_glWidget = NULL;
}

int XYZMCodec::getDecodeStreamWidth(void)
{
  return 640;
}

int XYZMCodec::getDecodeStreamHeight(void)
{
  return 480;
}

float XYZMCodec::getDecodeStreamProgress(void)
{
  //return m_io.readStreamPosition();
  return 100;
}

string XYZMCodec::codecName(void)
{
  return "XYZM Codec";
}
