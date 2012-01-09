#include "MultiWavelengthDecoder.h"

MultiWavelengthDecoder::MultiWavelengthDecoder(string& filename)
{
  m_filename = filename;
}

void MultiWavelengthDecoder::openCodec(EncodingOpenGLWidget* glWidget)
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
  m_glWidget->reinit(getWidth(), getHeight());
}

void MultiWavelengthDecoder::closeCodec(void)
{
  m_io.closeReadStream();
  m_glWidget = NULL;
}

void MultiWavelengthDecoder::process(MeshInterchange* data)
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a NULL MeshInterchange
    data = NULL;
    return;
  }

  if(_streamUntilNewFrame())
  {
    //  End of the file. Return a NULL mesh
    data = NULL;
    return;
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

  data = m_glWidget->decode();
}

void MultiWavelengthDecoder::previewProcess(MeshInterchange* data)
{
}

bool MultiWavelengthDecoder::_streamUntilNewFrame(void)
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

int MultiWavelengthDecoder::getWidth(void)
{
  return m_io.readStreamWidth();
}

int MultiWavelengthDecoder::getHeight(void)
{
  return m_io.readStreamHeight();
}

float MultiWavelengthDecoder::getStreamLocation(void)
{
  return m_io.readStreamPosition();
}

void MultiWavelengthDecoder::setGammaCutoff(float gammaValue)
{
  m_coder.setGammaCutoff(gammaValue);
}

void MultiWavelengthDecoder::setScalingFactor(float scaling)
{
  m_coder.setScalingFactor(scaling);
}

string MultiWavelengthDecoder::codecName(void)
{
  return "Multi Wavelength Decoder";
}
