#include "DepthCodec.h"

void DepthCodec::openEncodeStream(int width, int height)
{
  //  Open the stream to write to
  m_io.openSaveStream("/home/karpinsn/tmp/Depth.avi", width, height, 30);
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
