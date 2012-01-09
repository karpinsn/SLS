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
  if(NULL == glWidget)
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
  m_glWidget = NULL;
}

void HolovideoDecoder::process(MeshInterchange* data)
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a NULL MeshInterchange
    data = NULL;
    return;
  }

  IplImage* frame = m_io.readStream();
  if(NULL == frame)
  {
	//	At the end of the file
    data = NULL;
    return;
  }

  m_decoder.setBackHoloBuffer(frame);
  m_decoder.swapBuffers();

  data = m_glWidget->decode();
}

void HolovideoDecoder::previewProcess(MeshInterchange* data)
{
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
