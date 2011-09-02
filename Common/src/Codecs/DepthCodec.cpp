#include "DepthCodec.h"

void DepthCodec::openEncodeStream(EncodingOpenGLWidget* glWidget, string& filename, int width, int height)
{
  //  Open the stream to write to
  m_io.openSaveStream(filename, width, height, 30);
}

void DepthCodec::encode(MeshInterchange& data)
{
  //  Encode to the stream
  m_io.saveStream(data);
}

void DepthCodec::closeEncodeStream(void)
{
  //  Close the stream
  m_io.closeSaveStream();
}

void DepthCodec::openDecodeStream(EncodingOpenGLWidget* glWidget, string& filename)
{
}

MeshInterchange* DepthCodec::decode()
{
  return NULL;
}

void DepthCodec::closeDecodeStream(void)
{

}
