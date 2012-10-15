#include "SaveStream.h"

SaveStream::SaveStream()
{	
  m_encoder = nullptr;
}

void SaveStream::open(shared_ptr<Holoencoder> encoder, shared_ptr<IOutputStream> outStream)
{
    m_encoder = encoder;
    m_outStream = outStream;

	m_outStream->Open();
}

void SaveStream::encodeAndStream(shared_ptr<MeshInterchange> mesh)
{
	m_encoder->setCurrentMesh(mesh);
	m_encoder->encode();
	shared_ptr<MeshInterchange> encodedMesh(m_encoder->getEncodedData());
	m_outStream->WriteStream(encodedMesh);
}

void SaveStream::close()
{
	m_outStream->Close();
}
