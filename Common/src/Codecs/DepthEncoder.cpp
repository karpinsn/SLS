#include "DepthEncoder.h"

void DepthEncoder::openCodec(EncodingOpenGLWidget* glWidget)
{
  //  TODO comeback and fix this
  string filename = "N:/Data/Blah.avi";

  //  Open the stream to write to
  m_io.openSaveStream(filename, m_width, m_height, 30);

  m_floatImageHandleThreeChannel = cvCreateImage(cvSize(getWidth(), getHeight()), IPL_DEPTH_32F, 3);
  m_floatImageHandleSingleChannel = cvCreateImage(cvSize(getWidth(), getHeight()), IPL_DEPTH_32F, 1);
  m_byteImageHandle = cvCreateImage(cvSize(getWidth(), getHeight()), IPL_DEPTH_8U, 1);
}

void DepthEncoder::closeCodec(void)
{
  //  Close the stream
  m_io.closeSaveStream();
}

void DepthEncoder::process(MeshInterchange* data)
{
  if(GL_FLOAT == data->getTexture()->getDataType()) // TODO comeback and fix this
  {
    //  Floating point texture that needs to be converted to 8 bit before being saved
    data->getTexture()->transferFromTexture(m_floatImageHandleThreeChannel);
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
    m_io.saveStream(*data);
  }
}

void DepthEncoder::previewProcess(MeshInterchange* data)
{
  return; //  TODO comeback and fix this
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
