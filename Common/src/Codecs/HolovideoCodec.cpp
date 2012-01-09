#include "HolovideoCodec.h"

HolovideoCodec::HolovideoCodec()
{
  m_width	  = -1;
  m_height	  = -1;
  m_filename  = "";
}

void HolovideoCodec::initCodec(string& filename, int width, int height)
{
  m_width	  = width;
  m_height	  = height;
  m_filename  = filename;
}

void HolovideoCodec::openEncodeStream(EncodingOpenGLWidget* glWidget)
{
  //  Open the stream to write to
  if(NULL == glWidget)
  {
	//	No OpenGL encoding widget. Cant open encoding stream
	return;
  }

  m_glWidget = glWidget;
  m_glWidget->setEncodingContext(&m_encoder);
  m_glWidget->setGLContext(&m_encoder);
  m_glWidget->reinit(m_width, m_height);

  m_io.openSaveStream(m_filename, m_width, m_height, 30);
}

void HolovideoCodec::encode(MeshInterchange& data)
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return;
  }

  m_encoder.setCurrentMesh(&data);

  MeshInterchange* mesh = m_glWidget->encode();

  //  Encode to the stream
  m_io.saveStream(*(mesh->getTexture()));
}

void HolovideoCodec::previewEncode(MeshInterchange& data)
{
  if(NULL == m_glWidget)
  {
    //  No OpenGL encoding widget. Return. Should error
    return;
  }

  m_encoder.setCurrentMesh(&data);

  //  Dont want to encode, only draw
  m_glWidget->updateScene();
}

void HolovideoCodec::closeEncodeStream(void)
{
  //  Close the stream
  m_io.closeSaveStream();
}

void HolovideoCodec::openDecodeStream(EncodingOpenGLWidget* glWidget)
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
