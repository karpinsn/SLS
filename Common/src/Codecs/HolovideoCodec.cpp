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

}

void HolovideoCodec::closeDecodeStream(void)
{

}
