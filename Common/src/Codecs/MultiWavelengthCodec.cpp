#include "MultiWavelengthCodec.h"

void MultiWavelengthCodec::openEncodeStream(string& filename, int width, int height)
{
  //  Open the stream to write to
  m_io.openSaveStream(filename, width, height, 30);
}

void MultiWavelengthCodec::encode(MeshInterchange& data)
{
  //  Encode to the stream
  m_io.saveStream(data);
}

void MultiWavelengthCodec::closeEncodeStream(void)
{
  //  Close the stream
  m_io.closeSaveStream();
}
