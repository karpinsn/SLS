#include "MultiWavelengthCodec.h"

MultiWavelengthCodec::MultiWavelengthCodec(string& filename)
{
  m_filename = filename;
}

void MultiWavelengthCodec::openEncodeStream(EncodingOpenGLWidget* glWidget)
{
  int width = 512;
  int height = 512;

  //  Open the stream to write to
  m_io.openSaveStream(m_filename, width, height, 30);
}

void MultiWavelengthCodec::previewEncode(MeshInterchange& data)
{
  return; //  TODO comeback and fix this
}

void MultiWavelengthCodec::encode(MeshInterchange& data)
{
  //  Encode to the stream
  m_io.saveStream(data);
}

void MultiWavelengthCodec::closeEncodeStream(void)
{
  //  Close the stream
  m_io.closeSaveStream();
}

void MultiWavelengthCodec::openDecodeStream(EncodingOpenGLWidget* glWidget)
{
  if(NULL == glWidget)
  {
    //  No OpenGL encoding widget. Cant open decode stream
    return;
  }

  m_io.openReadStream(m_filename);
  m_calculateReference = true;
  m_glWidget = glWidget;

  m_glWidget->setGLContext(&m_coder);
  m_glWidget->setDecodingContext(&m_coder);
  m_glWidget->reinit(getDecodeStreamWidth(), getDecodeStreamHeight());
}

MeshInterchange* MultiWavelengthCodec::decode()
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a NULL MeshInterchange
    return NULL;
  }

  if(_streamUntilNewFrame())
  {
    //  End of the file. Return a NULL mesh
    return NULL;
  }

  if(m_calculateReference)
  {
	//	Capture the reference plane
    m_calculateReference = false;
    m_coder.captureReferencePlane();
	m_glWidget->decode();

	//	Stream again until we get an actual frame
	_streamUntilNewFrame();
  }

  return m_glWidget->decode();
}

void MultiWavelengthCodec::closeDecodeStream(void)
{
  m_io.closeReadStream();
  m_glWidget = NULL;
}

bool MultiWavelengthCodec::_streamUntilNewFrame(void)
{
  IplImage* frame = m_io.readStream();

  //  While the frame is not null
  //  and we dont have a new 3D frame
  while(NULL != frame && !m_coder.newImage(frame))
  {
    frame = m_io.readStream();
  }

  return frame == NULL;
}

int MultiWavelengthCodec::getDecodeStreamWidth(void)
{
  return m_io.readStreamWidth();
}

int MultiWavelengthCodec::getDecodeStreamHeight(void)
{
  return m_io.readStreamHeight();
}

float MultiWavelengthCodec::getDecodeStreamProgress(void)
{
  return m_io.readStreamPosition();
}

void MultiWavelengthCodec::setGammaCutoff(float gammaValue)
{
  m_coder.setGammaCutoff(gammaValue);
}

void MultiWavelengthCodec::setScalingFactor(float scaling)
{
  m_coder.setScalingFactor(scaling);
}

string MultiWavelengthCodec::codecName(void)
{
  return "Multi Wavelength Codec";
}
