#include "SaveStream.h"

SaveStream::SaveStream()
{	
  m_encoder = nullptr;
}

void SaveStream::open(shared_ptr<Codec> encoder, shared_ptr<IOutputStream> outStream)
{
    m_encoder = encoder;
    m_outStream = outStream;
}

void SaveStream::encodeAndStream(MeshInterchange* mesh)
{

}

void SaveStream::close()
{

}
