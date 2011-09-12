#include "HolovideoCodec.h"

void HolovideoCodec::openEncodeStream(EncodingOpenGLWidget* glWidget, string& filename, int width, int height)
{
  //  Open the stream to write to
  m_io.openSaveStream(filename, width, height, 30);
}

void HolovideoCodec::encode(MeshInterchange& data)
{
  //  Encode to the stream
  m_io.saveStream(data);
}

void HolovideoCodec::closeEncodeStream(void)
{
  //  Close the stream
  m_io.closeSaveStream();
}

void HolovideoCodec::openDecodeStream(EncodingOpenGLWidget* glWidget, string& filename)
{
  if(NULL == glWidget)
  {
	//  No OpenGL encoding widget. Cant open decode stream
	return;
  }

  m_io.openReadStream(filename);
  m_glWidget = glWidget;

  m_glWidget->setGLContext(&m_decoder);
  m_glWidget->setEncodingContext(&m_decoder);
  m_glWidget->reinit(getDecodeStreamWidth(), getDecodeStreamHeight());
}

MeshInterchange* HolovideoCodec::decode()
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return a NULL MeshInterchange
    return NULL;
  }

  IplImage* frame = m_io.readStream();
  if(NULL == frame)
  {
	//	At the end of the file
	return NULL;
  }

  m_decoder.setBackHoloBuffer(frame);
  m_decoder.swapBuffers();

  return m_glWidget->decode();
}

void HolovideoCodec::closeDecodeStream(void)
{
  m_io.closeReadStream();
  m_glWidget = NULL;
}

int HolovideoCodec::getDecodeStreamWidth(void)
{
  return m_io.readStreamWidth();
}

int HolovideoCodec::getDecodeStreamHeight(void)
{
  return m_io.readStreamHeight();
}

float HolovideoCodec::getDecodeStreamProgress(void)
{
  return m_io.readStreamPosition();
}

string HolovideoCodec::codecName(void)
{
  return "Holovideo Codec";
}
