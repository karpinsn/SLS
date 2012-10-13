#include "HolovideoDecoder.h"

HolovideoDecoder::HolovideoDecoder()
{
  m_filename  = "";
}

void HolovideoDecoder::initCodec(string& filename)
{
  m_filename  = filename;
}

void HolovideoDecoder::openCodec(EncodingOpenGLWidget* glWidget)
{
  if(nullptr == glWidget)
  {
	//  No OpenGL encoding widget. Cant open decode stream
	return;
  }

  m_io.openReadStream(m_filename);
  m_glWidget = glWidget;

  m_glWidget->setGLContext(&m_decoder);
  m_glWidget->setDecodingContext(&m_decoder);
  m_glWidget->reinit(getWidth(), getHeight());
}

void HolovideoDecoder::closeCodec(void)
{
  m_io.closeReadStream();
  m_glWidget = nullptr;
}

shared_ptr<MeshInterchange> HolovideoDecoder::process(shared_ptr<MeshInterchange> data)
{
  if(nullptr == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a nullptr MeshInterchange
    return nullptr;
  }

  IplImage* frame = m_io.readStream();
  if(nullptr == frame)
  {
	//	At the end of the file
    return nullptr;
  }

  m_decoder.setBackHoloBuffer(frame);
  m_decoder.swapBuffers();

  return shared_ptr<MeshInterchange>(m_glWidget->decode());
}

shared_ptr<MeshInterchange> HolovideoDecoder::previewProcess(shared_ptr<MeshInterchange> data)
{
  return nullptr;
}

int HolovideoDecoder::getWidth(void)
{
  return m_io.readStreamWidth();
}

int HolovideoDecoder::getHeight(void)
{
  return m_io.readStreamHeight();
}

float HolovideoDecoder::getStreamLocation(void)
{
  return m_io.readStreamPosition();
}

string HolovideoDecoder::codecName(void)
{
  return "Holovideo Decoder";
}
