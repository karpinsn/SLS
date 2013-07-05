#include "Codecs/DepthEncoder.h"

void DepthEncoder::openCodec(EncodingOpenGLWidget* glWidget)
{
  //  Open the stream to write to
  m_io.openSaveStream(m_filename, m_width, m_height, 30);

  m_floatImageHandleThreeChannel = cvCreateImage(cvSize(getWidth(), getHeight()), IPL_DEPTH_32F, 3);
  m_floatImageHandleSingleChannel = cvCreateImage(cvSize(getWidth(), getHeight()), IPL_DEPTH_32F, 1);
  m_byteImageHandle = cvCreateImage(cvSize(getWidth(), getHeight()), IPL_DEPTH_8U, 1);
}

void DepthEncoder::closeCodec(void)
{
  //  Close the stream
  m_io.closeSaveStream();
}

shared_ptr<MeshInterchange> DepthEncoder::process(shared_ptr<MeshInterchange> data)
{
  if(GL_FLOAT == data->getTexture()->getDataType()) // TODO comeback and fix this
  {
    //  Floating point texture that needs to be converted to 8 bit before being saved
    data->getTexture()->transferFromTexture(m_floatImageHandleThreeChannel);
	cvSetImageCOI(m_floatImageHandleThreeChannel, 1);
	cvCopy(m_floatImageHandleThreeChannel, m_floatImageHandleSingleChannel);
	
	if(m_stretchContrast)
	{
	  float scale = 255.0/(m_maxContrastValue - m_minContrastValue);
	  cvConvertScale(m_floatImageHandleSingleChannel, m_byteImageHandle, scale, -m_minContrastValue * scale);
	}
	else
	{
	  cvConvertScale(m_floatImageHandleSingleChannel, m_byteImageHandle, 255.0);
	}

	MeshInterchange mesh(m_byteImageHandle, false);
	m_io.saveStream(mesh);
  }
  else
  {
	//  Encode to the stream
    m_io.saveStream(*data);
  }

  return nullptr;
}

shared_ptr<MeshInterchange> DepthEncoder::previewProcess(shared_ptr<MeshInterchange> data)
{
  return nullptr; //  TODO comeback and fix this
}

void DepthEncoder::setFilename(string filename)
{
  m_filename = filename;
}

void DepthEncoder::enableContrastStretching(float min, float max)
{
  m_stretchContrast = true;
  m_minContrastValue = min;
  m_maxContrastValue = max;
}

void DepthEncoder::disableContrastStretching()
{
  m_stretchContrast = false;
}

string DepthEncoder::codecName(void)
{
  return "Depthmap Encoder";
}
