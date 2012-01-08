#include "DepthCodec.h"

void DepthCodec::openEncodeStream(EncodingOpenGLWidget* glWidget)
{
  //  TODO comeback and fix this
  string filename = "";
  int width = 512;
  int height = 512;

  //  Open the stream to write to
  m_io.openSaveStream(filename, width, height, 30);

  m_floatImageHandleThreeChannel = cvCreateImage(cvSize(width, height), IPL_DEPTH_32F, 3);
  m_floatImageHandleSingleChannel = cvCreateImage(cvSize(width, height), IPL_DEPTH_32F, 1);
  m_byteImageHandle = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
}

void DepthCodec::previewEncode(MeshInterchange& data)
{
  return; //  TODO comeback and fix this
}

void DepthCodec::encode(MeshInterchange& data)
{
  if(GL_FLOAT == data.getTexture()->getDataType()) // TODO comeback and fix this
  {
    //  Floating point texture that needs to be converted to 8 bit before being saved
    data.getTexture()->transferFromTexture(m_floatImageHandleThreeChannel);
	cvSetImageCOI(m_floatImageHandleThreeChannel, 1);
	cvCopy(m_floatImageHandleThreeChannel, m_floatImageHandleSingleChannel);

	if(m_stretchContrast)
	{
	  cvConvertScale(m_floatImageHandleSingleChannel, m_byteImageHandle, 256.0/(m_maxContrastValue - m_minContrastValue), -m_minContrastValue);
	}
	else
	{
	  cvConvertScale(m_floatImageHandleSingleChannel, m_byteImageHandle, 256.0);
	}

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

void DepthCodec::enableContrastStretching(float min, float max)
{
  m_stretchContrast = true;
  m_minContrastValue = min;
  m_maxContrastValue = max;
}

void DepthCodec::disableContrastStretching()
{
  m_stretchContrast = false;
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