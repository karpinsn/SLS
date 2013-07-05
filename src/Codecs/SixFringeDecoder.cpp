#include "Codecs/SixFringeDecoder.h"

SixFringeDecoder::SixFringeDecoder(string& filename)
{
  m_filename = filename;
  m_returnImage = -1;
}

void SixFringeDecoder::openCodec(EncodingOpenGLWidget* glWidget)
{
  if(nullptr == glWidget)
  {
    //  No OpenGL encoding widget. Cant open decode stream
    return;
  }

  m_io.openReadStream(m_filename);
  m_calculateReference = true;
  m_glWidget = glWidget;

  //  Need to set the size on the coder and our OpenGL widget
  m_coder.init(getWidth(), getHeight());
  m_glWidget->setDecodingContext(&m_coder);
  m_glWidget->reinit(getWidth(), getHeight());
}

void SixFringeDecoder::closeCodec(void)
{
  m_io.closeReadStream();
  m_glWidget = nullptr;
}

shared_ptr<MeshInterchange> SixFringeDecoder::process(shared_ptr<MeshInterchange> data)
{
  if(nullptr == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a nullptr MeshInterchange
    return nullptr;
  }

  if(_streamUntilNewFrame())
  {
    //  End of the file. Return a nullptr mesh
    return nullptr;
  }

  if(m_calculateReference)
  {
	if(nullptr != m_shortReference && nullptr != m_longReference)
	{
	  m_coder.loadReferencePlane(this, &SixFringeDecoder::_referenceFrameCallback);
	  m_glWidget->decode();
	}
	else
	{
	  //	Capture the reference plane
	  m_coder.captureReferencePlane();
	  m_glWidget->decode();

	  //	Stream again until we get an actual frame
	  _streamUntilNewFrame();
	}
	m_calculateReference = false;
  }

  return shared_ptr<MeshInterchange>(m_glWidget->decode());
}

shared_ptr<MeshInterchange> SixFringeDecoder::previewProcess(shared_ptr<MeshInterchange> data)
{
  return nullptr;
}

bool SixFringeDecoder::_streamUntilNewFrame(void)
{
  IplImage* frame = m_io.readStream();

  //  While the frame is not null
  //  and we dont have a new 3D frame
  while(nullptr != frame && !m_coder.newImage(frame))
  {
    frame = m_io.readStream();
  }

  return frame == nullptr;
}

int SixFringeDecoder::getWidth(void)
{
  return m_io.readStreamWidth();
}

int SixFringeDecoder::getHeight(void)
{
  return m_io.readStreamHeight();
}

float SixFringeDecoder::getStreamLocation(void)
{
  return m_io.readStreamPosition();
}

void SixFringeDecoder::setReferencePlane(shared_ptr<IplImage> shortWavelength, shared_ptr<IplImage> longWavelength)
{
  m_shortReference = shortWavelength;
  m_longReference = longWavelength;
}

void SixFringeDecoder::setGammaCutoff(float gammaValue)
{
  m_coder.setGammaCutoff(gammaValue);
}

void SixFringeDecoder::setScalingFactor(float scaling)
{
  m_coder.setScalingFactor(scaling);
}

shared_ptr<IplImage> SixFringeDecoder::_referenceFrameCallback(void* callbackInstance)
{
  SixFringeDecoder* decoder = (SixFringeDecoder*)callbackInstance;
  decoder->m_returnImage = (decoder->m_returnImage + 1) % 2;
  return decoder->_referenceFrame(decoder->m_returnImage);
}

shared_ptr<IplImage> SixFringeDecoder::_referenceFrame(int returnImage)
{
	return 0 == returnImage ? m_shortReference : m_longReference;
}

string SixFringeDecoder::codecName(void)
{
  return "Six Fringe Decoder";
}
