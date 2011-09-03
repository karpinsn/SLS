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

}

MeshInterchange* HolovideoCodec::decode()
{
  return NULL;
}

void HolovideoCodec::closeDecodeStream(void)
{

}

int HolovideoCodec::getDecodeStreamWidth(void)
{
  return 0;
}

int HolovideoCodec::getDecodeStreamHeight(void)
{
  return 0;
}

float HolovideoCodec::getDecodeStreamProgress(void)
{
  return m_io.readStreamPosition();
}

string HolovideoCodec::codecName(void)
{
  return "Holovideo Codec";
}
