#include "DepthCodec.h"

void DepthCodec::openEncodeStream(EncodingOpenGLWidget* glWidget, string& filename, int width, int height)
{
  //  Open the stream to write to
  m_io.openSaveStream(filename, width, height, 30);

  m_floatImageHandle = cvCreateImage(cvSize(width, height), IPL_DEPTH_32F, 3);
  m_floatImageHandle2 = cvCreateImage(cvSize(width, height), IPL_DEPTH_32F, 1);
  m_byteImageHandle = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
}

void DepthCodec::encode(MeshInterchange& data)
{
  if(GL_FLOAT == data.m_texture->getDataType()) // TODO comeback and fix this
  {
	float min = .0;
	float max = .1;

    //  Floating point texture that needs to be converted to 8 bit before being saved
    data.m_texture->transferFromTexture(m_floatImageHandle);
	cvSetImageCOI(m_floatImageHandle, 1);
	cvCopy(m_floatImageHandle, m_floatImageHandle2);

    cvConvertScale(m_floatImageHandle2, m_byteImageHandle, 256.0/(max - min), -min);

	MeshInterchange mesh(m_byteImageHandle);
	m_io.saveStream(mesh);
  }
  else
  {
	//  Encode to the stream
	m_io.saveStream(data);
  }
}

void DepthCodec::closeEncodeStream(void)
{
  //  Close the stream
  m_io.closeSaveStream();
}

void DepthCodec::openDecodeStream(EncodingOpenGLWidget* glWidget, string& filename)
{
}

MeshInterchange* DepthCodec::decode()
{
  return NULL;
}

void DepthCodec::closeDecodeStream(void)
{

}

int DepthCodec::getDecodeStreamWidth(void)
{
  return m_io.readStreamWidth();
}

int DepthCodec::getDecodeStreamHeight(void)
{
  return m_io.readStreamHeight();
}

float DepthCodec::getDecodeStreamProgress(void)
{
  return m_io.readStreamPosition();
}

string DepthCodec::codecName(void)
{
  return "Depthmap Codec";
}