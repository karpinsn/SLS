#include "NineFringeDecoder.h"

NineFringeDecoder::NineFringeDecoder(string& filename)
{
  m_filename = filename;
}

void NineFringeDecoder::openCodec(EncodingOpenGLWidget* glWidget)
{
  if(nullptr == glWidget)
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

void NineFringeDecoder::closeCodec(void)
{
  m_io.closeReadStream();
  m_glWidget = nullptr;
}

shared_ptr<MeshInterchange> NineFringeDecoder::process(shared_ptr<MeshInterchange> data)
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
	//	Capture the reference plane
    m_calculateReference = false;
    m_coder.captureReferencePlane();
	m_glWidget->decode();

	//	Stream again until we get an actual frame
	_streamUntilNewFrame();
  }

  return shared_ptr<MeshInterchange>(m_glWidget->decode());
}

shared_ptr<MeshInterchange> NineFringeDecoder::previewProcess(shared_ptr<MeshInterchange> data)
{
  return nullptr;
}

bool NineFringeDecoder::_streamUntilNewFrame(void)
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

int NineFringeDecoder::getWidth(void)
{
  return m_io.readStreamWidth();
}

int NineFringeDecoder::getHeight(void)
{
  return m_io.readStreamHeight();
}

float NineFringeDecoder::getStreamLocation(void)
{
  return m_io.readStreamPosition();
}

void NineFringeDecoder::setGammaCutoff(float gammaValue)
{
  m_coder.setGammaCutoff(gammaValue);
}

void NineFringeDecoder::setScalingFactor(float scaling)
{
  m_coder.setScalingFactor(scaling);
}

string NineFringeDecoder::codecName(void)
{
  return "Nine Fringe Decoder";
}
